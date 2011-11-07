/* unbing - a SPMP8000 BIN game file utility
 *
 * Copyright (C) 2010, alemaxx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdint.h>
#include <stdio.h>
#include "Des.h"

#define ERR(msg) { \
	printf("error: %s\n", msg); \
	goto done; \
}

char aucKey[] = {0x6E, 0x2D, 0x70, 0x2A, 0x38, 0x4B, 0x47, 0x4D};

void hexdump(uint8_t *data, int length);

int main(int argc, char **argv)
{
	FILE *f = NULL;
	FILE *fout = NULL;
	int size, totalsize, i;
	uint8_t buff[0x400];
	uint8_t data[0x400];
	uint8_t data2[0x400];

	if (argc < 2) ERR("to few arguments");
	if ((f = fopen(argv[1], "rb")) == NULL) ERR("cannot open file");

	fseek(f, 0x80, SEEK_SET);

	totalsize = fread(buff, 1, 0x400, f);
	if (!Des_Go((char*)data, (char*)buff, 0x400, aucKey, 8, DECRYPT)) ERR("decrypt failed");
/*	if (!Des_Go((char*)data2, (char*)data, 0x400, aucKey, 8, ENCRYPT)) ERR("encrypt failed");

	for (i=0; i<0x400; i++) {
		if (buff[i] != data2[i]) {
			printf("mismatch at %x\n");
			break;
		}
	}
	if (i == 0x400) printf("encoding/decoding works!\n");
	else hexdump(data2, 0x400);

	goto done;

	hexdump(data, 0x400);
	goto done;
*/
	fseek(f, 0x480, SEEK_SET);
	fout = fopen(argv[2], "wb");
	fwrite(data, 0x400, 1, fout);
//	totalsize = 0;
	while (1) { //!feof(f)) {
		size = fread(buff, 1, 0x400, f);
//		printf(".");
		totalsize += size;
		fwrite(buff, size, 1, fout);
		if (size < 0x400) break;
	}
	fclose(fout);
	printf("%d bytes copied (%d)\n", totalsize, feof(f));

done:
	if (f != NULL) fclose(f);
	return 0;
}

void hexdump(uint8_t *data, int length)
{
	int i, j;

	printf("00000000 : ");
	for (i=0; i<length; i++) {
		if ((i != 0) && ((i & 0xF) == 0)) {
			printf("- ");
			for (j=0; j<16; j++) if ((data[i-16+j] > 0x1f) && (data[i-16+j] < 0x80)) printf("%c", data[i-16+j]);
			else printf(".");
			printf("\n%08x : ", i);
		}
		printf("%02x ", data[i]);
	}
	printf("\n");
}

