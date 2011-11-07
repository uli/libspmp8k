
/*
 * sprdump.c - rom image extractor for SPMP8k ROM images
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>


#define ROM_MAGIC	0x526F6D2E

typedef struct rom_header {
	uint32_t	magic;
	uint32_t	num_entries;
	uint32_t	unk[6];
} rom_header_t;

typedef struct rom_entry {
	uint32_t	flags;
	uint32_t	unk2;
	uint32_t	unk3;
	uint32_t	size;
	uint32_t	unk4;
	uint32_t	offs;
	char		str[8];	// not zero terminated!
} rom_entry_t;

typedef struct romdir_entry {
	int		n;
	int		next;
	char	name[24];
} romdir_entry_t;


#define	REF_DIRENTRY	1
#define	REF_DATA		8

char currpath[1024];

void usage(char *me);
void dump_entry(rom_entry_t *entries, int nentry, int list, FILE *f, char *name);

int main(int argc, char **argv)
{
	FILE	*fin, *fout;
	rom_header_t hdr;
	rom_entry_t  entry;
	rom_entry_t *entries;
	int  i, nfile;
	char ofname[255];

	printf("%s - spmp8k rom image dumper\n(alemaxx at hotmail.de)\n", argv[0]);

	if (argc < 3) {
		printf("error: to few args\n");
		usage(argv[0]);
		return 0;
	}

	if (!((argv[1][0] == 'x')  || (argv[1][0] == 'l'))) {
		usage(argv[0]);
		return;
	}

	fin = fopen(argv[2], "rb");
	if (fin == NULL) {
		printf("error: couldnt open file \"%s\"\n", argv[1]);
		return 0;
	}

	fread(&hdr, sizeof(rom_header_t), 1, fin);
	if (hdr.magic != ROM_MAGIC) {
		printf("error: not a rom file\n");
		fclose(fin);
		return 0;
	}
	printf("%d files in rom file\n", hdr.num_entries);
	entries = (rom_entry_t*)malloc(hdr.num_entries * sizeof(rom_entry_t));
	fread(entries, sizeof(rom_entry_t), hdr.num_entries, fin);

	if (entries[0].flags & REF_DIRENTRY) {
		currpath[0] = 0;
		if (argv[1][0] == 'x') {
			if (argc < 4) {
				printf("error: too few parameters\n");
				usage(argv[0]);
				goto done;
			}
			strcpy(currpath, argv[3]);
			if (currpath[strlen(currpath)-1] == '/') currpath[strlen(currpath)-1] = '\0';
			printf("extracting files to %s\n", currpath);
			dump_entry(entries, 0, 0, fin, "");
		}
		else if (argv[1][0] == 'l') dump_entry(entries, 0, 1, fin, ".");
	}
	else printf("error: first entry is not a directory entry!\n");

done:
	free(entries);
	fclose(fin);

	return 0;
}

void dump_entry(rom_entry_t *entries, int nentry, int list, FILE *f, char *name)
{
	if (entries[nentry].flags & REF_DIRENTRY) {
		romdir_entry_t *romdir;
		int i, j, n_direntries;

		romdir = (romdir_entry_t*)malloc(entries[nentry].size);
		fseek(f, entries[nentry].offs, SEEK_SET);
		fread(romdir, entries[nentry].size, 1, f);
		n_direntries = entries[nentry].size >> 5;

		strcat(currpath, name);
		strcat(currpath, "/");

		if (list == 0) {
			char cmd[1024];
			struct stat st;
			strcpy(cmd, currpath);
			if (stat(cmd, &st)) {
				printf("creating directory %s\n", currpath);
				sprintf(cmd, "mkdir %s", currpath);
				system(cmd);
			}
		}

		for (i=2; i<n_direntries; i++) {
			if (list == 1) printf("%s%s\n", currpath, romdir[i].name);
			if (romdir[i].n == 0) {
				printf("%d links to 0\n", nentry);	// check for other inconsistencies (might cause infinite loops)
			}
			else {
				dump_entry(entries, romdir[i].n, list, f, romdir[i].name);
			}
		}
		currpath[strlen(currpath)-1] = '\0';
		if (strrchr(currpath, '/') == NULL) printf("whoops!\n");
		else *(strrchr(currpath, '/') + 1) = '\0';

		free(romdir);
	}
	else if ((entries[nentry].flags & REF_DATA)) {
		if (list == 0) {
			// extract file
			char fname[1024];
			uint8_t *buff;
			FILE *fout;


			sprintf(fname, "%s%s", currpath, name);
			fout = fopen(fname, "wb");
			if (fout == NULL) {
				printf("error: couldnt create file \"%s\"\n", fname);
				return;
			}
			buff = (uint8_t*)malloc(entries[nentry].size);
			if (buff == NULL) {
				printf("error: couldnt alloc %d bytes\n", entries[nentry].size);
				exit(0);
			}
			fseek(f, entries[nentry].offs, SEEK_SET);
			fread(buff, entries[nentry].size, 1, f);
			fwrite(buff, entries[nentry].size, 1, fout);
			fclose(fout);
			free(buff);
		}
	}
	else {
		int i;
		printf("unknown entry (%d, %08X)\n", nentry, entries[nentry].flags);
		uint8_t *buff = (uint8_t*)&entries[nentry];
		for (i=0; i<32; i++) printf("%02x ", buff[i]);
		printf("\n");
	}
}

void usage(char *me)
{
	printf("\tusage: %s x file.rom dir (extracts file to dir)\n", me);
	printf("\t   or  %s l file.rom     (list the contents)\n\n", me);
}

