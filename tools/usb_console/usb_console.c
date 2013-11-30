/*
 * libgame USB debug console
 *
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

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>

#define MAGIC_BLOCK 3072
#define READ_SIZE 512

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <block device>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_DIRECT|O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  char *buf = mmap(0, READ_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (buf == (char *)MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  for (;;) {
    if (lseek(fd, MAGIC_BLOCK * 512, SEEK_SET) < 0) {
      perror("lseek");
      return -1;
    }
    if (read(fd, buf, READ_SIZE) < 0) {
      perror("read");
      return 1;
    }
    uint32_t len = ((uint32_t *)buf)[0];
    if (len > 0) {
      write(1, buf + sizeof(uint32_t), len);
    }
    else {
      usleep(100000);
    }
  }
}
