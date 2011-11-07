/* mkbing - a SPMP8000 BIN game file utility
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
#include <stdlib.h>
#include <strings.h>
#include <memory.h>
#include "Des.h"

#define ERR(msg) { \
	printf("error: %s\n", msg); \
	goto done; \
}

char aucKey[] = {0x6E, 0x2D, 0x70, 0x2A, 0x38, 0x4B, 0x47, 0x4D};

#define	GAME_NAME		0x2c
#define	LCM_MODE		0x6d
#define	GAME_SZICON		0x78	// its possible to append a JPEG icon (64x64 pixels)
#define	GNAME_MAXLEN	15

uint8_t g_header[] = {
	0x4e, 0x47, 0x61, 0x6d, 0x65, 0x31, 0x2e, 0x30,	// 0000
	0x80, 0x00, 0x00, 0x00, 0x53, 0x75, 0x6e, 0x70, // |NGame1.0....Sunp|
	0x6c, 0x75, 0x73, 0x00, 0xfd, 0xfd, 0xfd, 0xfd,	// 0010
	0x41, 0x00, 0x00, 0x00, 0x53, 0x50, 0x43, 0x41, // |lus.....A...SPCA|
	0x35, 0x35, 0x36, 0x00, 0xfd, 0xfd, 0xfd, 0xfd,	// 0020
	0x41, 0x00, 0x00, 0x00, 0x4a, 0x75, 0x73, 0x74, // |556.....A...Just|
	0x41, 0x47, 0x61, 0x6d, 0x65, 0x21, 0x00, 0xfd,	// 0030
	0xfd, 0xfd, 0xfd, 0x00, 0x51, 0x01, 0x00, 0x00, // |AGame!......Q...|
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,	// 0040
	0x51, 0x00, 0x00, 0x00, 0x50, 0x75, 0x6e, 0x6d, // |........Q...Punm|
	0x65, 0x64, 0x69, 0x61, 0x00, 0x72, 0x00, 0xfd,	// 0050
	0xfd, 0xfd, 0xfd, 0x00, 0x10, 0x00, 0x00, 0x00, // |edia.r..........|
	0x1c, 0x21, 0x39, 0x00, 0x1c, 0x21, 0x39, 0x00,	// 0060
	0x51, 0x00, 0x00, 0x00, 0x05, 0x05, 0x02, 0x00, // |.!9..!9.Q.......|
	0x02, 0x00, 0x01, 0x00, 0x00, 0x31, 0x32, 0x32,	// 0070
	0xa6, 0x5d, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, // |.....122.]..@.@.|
	0xb6, 0xf7, 0xd1, 0x9a, 0xe5, 0x3e, 0x7b, 0xe0,	// 0080
	0x3d, 0x20, 0x18, 0xaf, 0x58, 0x1b, 0x1e, 0xb3  // |.....>{.= ..X...|
};

char g_game_ident[] = "SUNPLUS556GAME\0\0";

// protoplasm
void silly_banner();
void usage();


int main(int argc, char **argv)
{
	FILE *fin, *fout;
	uint8_t hdr[0x80];
	char *iname = NULL;
	char *oname = NULL;
	uint8_t ibuff[0x400], obuff[0x400];
	int size, totalsize;
	int i;

	silly_banner();

	if (argc < 3) {
		printf("error: too few parameters\n\n");
		usage();
		return 0;
	}

	// init the header
	memcpy(hdr, g_header, 0x80);
	uint32_t *icon_size = (uint32_t*)&hdr[GAME_SZICON];
	*icon_size = 0;

	// parse arguments
	i = 1;
	while (i < argc) {
		if (argv[i][0] == '-') {
			if (i >= (argc-1)) {
				usage();
				return 0;
			}
			switch (argv[i][1]) {
				case 'h':
				{
					FILE *fhead;
					if ((fhead = fopen(argv[i+1], "rb")) == NULL) {
						printf("error: unable to open custom header file \"%s\"\n", argv[i+1]);
						return 0;
					}
					if (fread(hdr, 1, 0x80, fhead) != 0x80) {
						printf("error: invalid header file\n");
						return 0;
					}
					fclose(fhead);
					break;
				}
				case 'n':
					if (strlen(argv[i+1]) > GNAME_MAXLEN) {
						printf("error: name is to long must <16 characters in length\n");
						return 0;
					}
					memcpy(&hdr[GAME_NAME], argv[i+1], strlen(argv[i+1])+1);
					break;
				case 's':
					hdr[LCM_MODE] = strtol(argv[i+1], NULL, 16);
					break;
				default:
					usage();
					return 0;
			}
			i+=2;
		}
		else {
			// we assume the next to arguments to be the infile and outfile
			if ((argc - i) < 2) {
				usage();
				return 0;
			}
			iname = argv[i];
			oname = argv[i+1];
			break;
		}
	}

	// check if we are ready to go (to togo)
	if ((iname == NULL) || (oname == NULL)) {
		printf("error: source and/or destination file(s) are unspecified\n");
		return 0;
	}

	// read input
	if ((fin = fopen(iname, "rb")) == NULL)   ERR("cannot open input file");
	memset(ibuff, 0, 0x400);
	if (fread(ibuff, 1, 0x400, fin) < 0x400) printf("its tiny! ;-)\n");

	// encrypt the 1st k
	if (!Des_Go((char*)obuff, (char*)ibuff, 0x400, aucKey, 8, ENCRYPT)) ERR("encryping failed");

	// write header and that 1k of encrypted data
	if ((fout = fopen(oname, "wb")) == NULL)    ERR("cannot open output file");
	if (fwrite(hdr, 1, 0x80, fout) != 0x80)     ERR("cannot write header to output");
	if (fwrite(obuff, 1, 0x400, fout) != 0x400) ERR("cannot write encrypted block to output");

	// copy the remaining stuff
	totalsize = 0x480;
	while (1) {
		size = fread(ibuff, 1, 0x400, fin);
		totalsize += size;
		fwrite(ibuff, size, 1, fout);
		if (size < 0x400) break;
	}
	totalsize += fwrite(g_game_ident, 16, 1, fout);
	printf("success!\n%d bytes of data have been written\n", totalsize);

done:
	if (fin != NULL) fclose(fin);
	if (fout != NULL) fclose(fout);

	return 0;
}

void usage()
{
	printf("  usage [options] infile outfile\n\n");
	printf("   -h file     use the supplied header file\n");
	printf("   -n name     patch the name field\n");
	printf("   -s val      patch the LCM resolution value (in hex)\n\n");
}

void silly_banner()
{
	printf("  ##############################\n");
	printf("  # mkbing - a bin game packer #\n");
	printf("  # 2010 alemaxx               #\n");
	printf("  ##############################\n\n");
}

