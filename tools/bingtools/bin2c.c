/* bin2c - binary file to c array converter
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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ERR(msg) { \
	printf("error: %s\n", msg); \
	goto done; \
}

int main(int argc, char **argv)
{
	char	filename[256];
	char	str[256];
	FILE	*fin = NULL;
	FILE	*fout = NULL;
	uint8_t	*data = NULL;
	int		size, i;

	if (argc < 3) ERR("to few arguments")

	if ((fin = fopen(argv[1], "rb")) == NULL) ERR("cannot open input");

	fseek(fin, 0, SEEK_END);
	size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	data = (uint8_t*)malloc(size);
	fread(data, 1, size, fin);

	// write the header file
	strcpy(filename, argv[2]);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	strcat(filename, ".h");
	if ((fout = fopen(filename, "w+")) == NULL) ERR("cannot create header file\n")
	strcpy(filename, argv[2]);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	for (i=0; i<strlen(argv[2]); i++) str[i] = toupper(filename[i]);
	fprintf(fout, "\n#ifndef __%s_H__\n#define __%s_H__\n\n", str, str);
	fprintf(fout, "#define %s_LENGTH\t%d\n\n", str, size);
	fprintf(fout, "extern char %s_data[];\n\n", filename);
	fprintf(fout, "#endif // __%s_H__\n\n", str);
	fclose(fout);
	fout = NULL;

	// write data to .c file
	strcpy(filename, argv[2]);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	strcat(filename, ".c");
	if ((fout = fopen(filename, "w+")) == NULL) ERR("cannot create header file\n")
	strcpy(filename, argv[2]);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	fprintf(fout, "\n#include \"%s.h\"\n\n", filename);
	fprintf(fout, "char %s_data[%s_LENGTH] = {", filename, str);
	for (i=0; i<size; i++) {
		if ((i & 0xf) == 0) fprintf(fout, "\n\t");
		if (i == (size-1)) fprintf(fout, "0x%02x\n};\n\n", data[i]);
		else fprintf(fout, "0x%02x, ", data[i]);
	}
	fclose(fout);
	fout = NULL;

done:
	if (fin != NULL) fclose(fin);
	if (fout != NULL) fclose(fout);
	if (data != NULL) free(data);

	return 0;
}

