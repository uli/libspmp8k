#include <libgame.h>
#include <stdio.h>
#include <string.h>

void my_putc(char c)
{
  char str[2];
  str[1] = 0;
  str[0] = c;
  usbdbg_puts(str);
}

void *old_putc;

int main()
{
  stderr = fopen("stderr_usb.txt", "w");
  fprintf(stderr, "USB test\n");

  /* Initialize USB debugging hack. */
  fprintf(stderr, "usbdbg_init %d\n", usbdbg_init());

  /* Write some diagnostic output to a file to help us if something
      doesn't work. */
  fprintf(stderr, "diag_printf %p\n", diag_printf);
  fprintf(stderr, "MCatchInitGraph %p\n", MCatchInitGraph);
  fprintf(stderr, "readBlksWithLock_SD %p\n", readBlksWithLock_SD);
  fprintf(stderr, "writeBlksWithLock_SD %p\n", writeBlksWithLock_SD);
  fprintf(stderr, "_rwstor2_msg_55a_call %p\n", _rwstor2_msg_55a_call);
  fprintf(stderr, "_rwstor2_msg_551_call %p\n", _rwstor2_msg_551_call);
  fprintf(stderr, "g_MwFlags %p\n", g_MwFlags);

  /* Redirect system debug output to USB. */
  old_putc = *_diag_putc;
  *_diag_putc = my_putc;

  /* Enable lots of system debug output. */
  libgame_set_debug(1);
  *g_MwFlags |= 4;

  /* Blabber something until the user presses X. */
  ge_key_data_t keys;
  char str[80];
  int count = 0;

  for (;;) {
    NativeGE_getKeyInput4Ntv(&keys);
    if (keys.keys & GE_KEY_X)
      break;

    cyg_thread_delay(2);

    sprintf(str, "debug buf test %d\n", count);
    usbdbg_puts(str);

    count++;
  }

  /* Disable system debugging and direct it back to the serial port. */
  libgame_set_debug(0);
  *_diag_putc = old_putc;

  fprintf(stderr, "ending app\n");
  fclose(stderr);

  /* Finalize USB debugging. Restores default USB behavior (hopefully). */
  usbdbg_fini();

  return 0;
}
