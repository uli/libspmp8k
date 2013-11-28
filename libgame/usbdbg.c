/*
 * Copyright (C) 2013, Ulrich Hecht <uli@emulinks.de>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "usbdbg.h"
#include "libgame.h"
#include "mcatch_cmd.h"
#include <stdint.h>
#include <string.h>

#define DEBUG_BUF_SIZE 8192
#define MAGIC_BLOCK 16

static char dbg_buf[DEBUG_BUF_SIZE];
static volatile unsigned int dbg_buf_start = 0;
static volatile unsigned int dbg_buf_end = 0;

/* We redirect the firmware's readSD_UDC() here; this function returns a
   buffer filled with 0x42 bytes by default, and the current contents
   of the USB debug ring buffer (up to the requested size) if the starting
   block is MAGIC_BLOCK, with the size of valid data prepended. */
static int debug_readsd(uint32_t lba, uint16_t numBlks, uint8_t *buf)
{
  int size = numBlks * 512;
  if (!size)
    return 0;
  if (lba == MAGIC_BLOCK) {
    memset(buf, 0x23, size);
    uint32_t *buf_size = (uint32_t *)buf;
    buf += sizeof(uint32_t);
    size -= sizeof(uint32_t);
    int count = 0;
    while (dbg_buf_start != dbg_buf_end && size) {
      *buf = dbg_buf[dbg_buf_start];
      buf++;
      dbg_buf_start = (dbg_buf_start + 1) % DEBUG_BUF_SIZE;
      size--;
      count++;
    }
    *buf_size = count;
  }
  else {
    memset(buf, 0x42, size);
  }
  return 0;
}

int usbdbg_blocking = 0;

void usbdbg_puts(const char *s)
{
  while (*s) {
    if (usbdbg_blocking) {
      while (dbg_buf_end == (dbg_buf_start - 1) % DEBUG_BUF_SIZE) {
        cyg_thread_delay(2);
      }
    }
    dbg_buf[dbg_buf_end] = *s;
    s++;
    dbg_buf_end = (dbg_buf_end + 1) % DEBUG_BUF_SIZE;
  }
}

/* We redirect the call from RWStor2_Thread_UDC() to PostMessageExt() with
   message ID 0x55a (0x499 on some older(?) firmwares) here because it is
   interpreted by Game_Proc() as an order to start the USB application and
   commit suicide.  Instead, we perform the necessary SPMP_SendSignal() call
   ourselves. */
static void my_PostMessageExt(uint32_t r0, uint32_t r1, uint32_t r2)
{
  /* NB: Parameters are not reliable; some firmware versions use a wrapper
     through a function pointer to call PostMessageExt. */
  (void)r0;
  (void)r1;
  (void)r2;

#if 0
  /* This is also done by Game_Proc() somewhere along the line (as a response
     to message 0x551, which we also disable), but it does not seem to be
     strictly necessary. */
  uint32_t unknown[8];
  unknown[6] = 10;
  unknown[7] = 1;
  SPMP_SendSignal(MCATCH_CMD_USB_SAVE_POWER_ON, unknown, 32);
#endif

  SPMP_SendSignal(MCATCH_CMD_USB_MODE_SET, "usb_test", 0);
}

static void invalidate_icache(void)
{
  __asm__("mcr p15, 0, %0, c7, c5, 0\n" :: "r"(0));
}

static uint32_t _rwstor2_msg_55a_call_orig = 0;
static uint32_t _rwstor2_msg_551_call_orig = 0;
static uint32_t readBlksWithLock_SD_prolog_orig = 0;

int usbdbg_init(void)
{
  /* check that all prerequisites are there */
  if (!_rwstor2_msg_55a_call || !_rwstor2_msg_551_call ||
      !readBlksWithLock_SD || !SPMP_SendSignal || !cyg_thread_delay)
    return -1;

  /* patch call to PostMessageExt(..., 0x55a, ...) */
  _rwstor2_msg_55a_call_orig = *_rwstor2_msg_55a_call;
  *_rwstor2_msg_55a_call = 0xeb000000 |	/* BL ... */
    (((void *)my_PostMessageExt - (((void *)_rwstor2_msg_55a_call) + 8)) >> 2);

  /* patch call to PostMessageExt(..., 0x551, ...) */
  _rwstor2_msg_551_call_orig = *_rwstor2_msg_551_call;
  *_rwstor2_msg_551_call = 0xe1a00000; /* NOP */

  /* redirect readBlksWithLock_SD() to debug_readsd() */
  readBlksWithLock_SD_prolog_orig = ((uint32_t *)readBlksWithLock_SD)[0];
  ((uint32_t *)readBlksWithLock_SD)[0] = 0xea000000 |	/* B ... */
    (((void *)debug_readsd - (readBlksWithLock_SD + 8)) >> 2);

  invalidate_icache();

  return 0;
}

void usbdbg_fini(void)
{
  if (_rwstor2_msg_55a_call_orig)
    *_rwstor2_msg_55a_call = _rwstor2_msg_55a_call_orig;
  if (_rwstor2_msg_551_call_orig)
    *_rwstor2_msg_551_call = _rwstor2_msg_551_call_orig;
  if (readBlksWithLock_SD_prolog_orig)
    ((uint32_t *)readBlksWithLock_SD)[0] = readBlksWithLock_SD_prolog_orig;

  invalidate_icache();

  SPMP_SendSignal(MCATCH_CMD_USB_SAVE_POWER_OFF, 0, 0);
}
