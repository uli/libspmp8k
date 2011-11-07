/* png2c - png to c array converter
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
#include <png.h>
#include <stdlib.h>

/*
#define	ERR(msg) { \
	printf("error: %s\n", msg); \
	goto i_am_done; \
}

#define	ERR2(msg) \
	printf("error: %s\n", msg); \
	goto i_am_done;
*/

#define	ERR(msg) { \
	printf("error: %s\n", msg); \
	if (fin != NULL) fclose(fin); \
	if (fout != NULL) fclose(fout); \
	return 0; \
}

#define	ERR2(msg) \
	printf("error: %s\n", msg); \
	if (fin != NULL) fclose(fin); \
	if (fout != NULL) fclose(fout); \
	return 0;

void write_binimg(char *fname, uint8_t *data, png_color *pal_data, int pal_size, int width, int height, int bpp);
void write_2c(char *name, uint8_t *data, png_color *pal_data, int pal_size, int width, int height, int bpp);

int main(int argc, char **argv)
{
	FILE *fin  = NULL;
	FILE *fout = NULL;
	char header[8];
	uint8_t *image_data = NULL;
	png_bytep *row_pointers = NULL;
	int i;

	if (argc < 3) ERR("too few arguments")

	if ((fin = fopen(argv[1], "rb")) == NULL) ERR("cannot open man, lame!");

	printf("%s opened\n", argv[1]);

	fread(header, 1, 8, fin);
	if (png_check_sig(header, 8) == 0) ERR("not a valid .png image")

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) ERR("sdf");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		ERR2("fssdfs");
	}

	if (setjmp(png_ptr->jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

	png_init_io(png_ptr, fin);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
	printf("width    : %d\n", info_ptr->width);
	printf("height   : %d\n", info_ptr->height);
	printf("bpp      : %d\n", info_ptr->bit_depth);
	printf("palette  : %d entries\n", info_ptr->num_palette);
	printf("rowbytes : %d\n", info_ptr->rowbytes);
/*	fclose(fin);
	return 0;

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		ERR2("fjsh");
	}
*/
	// optional call to update palette with transformations
//	png_start_read_image(png_ptr);

	// optional call to update the info structure
//	png_read_update_info(png_ptr, info_ptr);
	if ((image_data = (uint8_t*)malloc(info_ptr->rowbytes*info_ptr->height)) == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*info_ptr->height);
	for (i=0;  i<info_ptr->height; ++i) row_pointers[i] = image_data + i*info_ptr->rowbytes;

	// the easiest way to read the image
	png_read_image(png_ptr, row_pointers);

	// void write_binimg(char *fname, uint8_t *data, png_color *pal_data, int pal_size, int width, int height, int bpp)
	char filename[256];
	strcpy(filename, argv[2]);
/*	write_binimg(strcat(filename, ".raw"), image_data, info_ptr->palette, info_ptr->num_palette,
		info_ptr->width, info_ptr->height, info_ptr->bit_depth);
*/
	write_2c(argv[2], image_data, info_ptr->palette, info_ptr->num_palette,
		info_ptr->width, info_ptr->height, info_ptr->bit_depth);

	// clean up after the read, and free any memory allocated
	png_read_destroy(png_ptr, info_ptr, (png_infop)0);
//	png_read_end(png_ptr, NULL);

i_am_done:
	if (image_data != NULL) free(image_data);
	if (row_pointers != NULL) free(row_pointers);
	if (fin != NULL) fclose(fin);
	if (fout != NULL) fclose(fout);

	return 0;
}

typedef struct img_hdr {
	uint16_t	type;
	uint16_t	pad[2];
	uint16_t	width;
	uint16_t	height;
	uint16_t	pad2;
	uint32_t	img_data;
	uint32_t	pal_struct;
} img_hdr_t;

typedef struct pal_hdr {
	uint16_t	type;
	uint16_t	num_entries;
	uint32_t	pal_offs;
} pal_hdr_t;

void write_2c(char *name, uint8_t *data, png_color *pal_data, int pal_size, int width, int height, int bpp)
{
	FILE *f;
	char filename[256];
	int length, i;
	uint16_t color;

	// only supporting 4bpp currently
	if (bpp != 4) return;

	strcpy(filename, name);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	strcat(filename, ".h");
	if ((f = fopen(filename, "wb")) == NULL) {
		printf("cannot open output(1)\n");
		return;
	}

	// write header file
	strcpy(filename, name);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	fprintf(f, "\n#include \"gfx_types.h\"\n\n");
	fprintf(f, "extern uint8_t %s_data[];\n", filename);
	fprintf(f, "extern uint16_t %s_pal[];\n\n", filename);
	fprintf(f, "extern gfx_loadimg_t %s_img;\n\n", filename);
	fclose(f);

	// write data
	length = width * height;
	strcpy(filename, name);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	strcat(filename, ".c");
	if ((f = fopen(filename, "wb")) == NULL) {
		printf("cannot open output(1)\n");
		return;
	}
	strcpy(filename, name);
	if (strchr(filename, '.') != NULL) *strchr(filename, '.') = '\0';
	fprintf(f, "\n#include \"%s.h\"\n\n", filename);
	fprintf(f, "gfx_loadimg_t %s_img = {", filename);
	fprintf(f, "%s_data, %d, %d, %d, 0, %s_pal, %d, 0xffffff80};\n\n",
		filename, width, height, 1, filename, pal_size);

	fprintf(f, "uint8_t %s_data[%d] = {", filename,length);
	length = length >> 1;
	for (i=0; i<length; i++) {
		if ((i & 0x7) == 0) fprintf(f, "\n\t");
		if (i == (length - 1)) {
			fprintf(f, "0x%02x, 0x%02x", (data[i] >> 4) & 0xf, data[i] & 0xf);
		}
		else {
			fprintf(f, "0x%02x, 0x%02x, ", (data[i] >> 4) & 0xf, data[i] & 0xf);
		}
	}
	fprintf(f, "\n};\n\n");
	fprintf(f, "uint16_t %s_pal[%d] = {", filename, pal_size);
	for (i=0; i<pal_size; i++) {
		if ((i & 0x7) == 0) fprintf(f, "\n\t");
		color = ((pal_data[i].red >> 3) << 11) | ((pal_data[i].green >> 2) << 5) | (pal_data[i].blue & 0x1f);
		if (i == (pal_size - 1)) fprintf(f, "0x%04x", color);
		else fprintf(f, "0x%04x, ", color);
	}
	fprintf(f, "\n};\n\n");
	fclose(f);
}

void write_binimg(char *fname, uint8_t *data, png_color *pal_data, int pal_size, int width, int height, int bpp)
{
	FILE *fout;
	img_hdr_t hdr;
	pal_hdr_t palhdr;
	uint16_t color, i;

	if (bpp != 4) return;

	fout = fopen(fname, "wb");

	hdr.type = 1;
	hdr.width = width;
	hdr.height = height;
	hdr.img_data = 0xA00000 + sizeof(img_hdr_t);
	hdr.pal_struct = 0xA00000 + (sizeof(img_hdr_t) + width * height);
	fwrite(&hdr, 1, sizeof(img_hdr_t), fout);

//	fwrite(data, width, height, fout);
	for (i=0; i<((width*height) >> 1); i++) {
		color = ((data[i] & 0xf) << 8)  | (data[i] >> 4);
		fwrite(&color, 1, 2, fout);
	}

	palhdr.type = 1;
	palhdr.num_entries = pal_size;
	palhdr.pal_offs = 0xA00000 + ftell(fout) + sizeof(pal_hdr_t);
	fwrite(&palhdr, 1, sizeof(pal_hdr_t), fout);
	for (i=0; i<pal_size; i++) {
		color = ((pal_data[i].red >> 3) << 11) | ((pal_data[i].green >> 2) << 5) | (pal_data[i].blue & 0x1f);
		fwrite(&color, 1, 2, fout);
	}
}

