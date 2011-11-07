/* FRMorp - USB IMG file dumper for SPMP8k
 *
 * Copyright (C) 2010, openschemes.com
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
#include <string.h>

#define DEBUG

#define SECTION_MAGIC 0xA0C7C1D4
#define BOOTFS_HEADMAGIC 0xE9D0CDCD


#define ERR(msg) { \
	printf("error %s\n", msg); \
	goto done; \
}

uint32_t lswap(uint32_t data){
	return (((data&0xFF)<<24) + ((data&0xFF00)<<8) + ((data&0xFF0000)>>8) + ((data&0xFF000000)>>24));
}

////////////////////////////////////////////////////////////////////////////
uint32_t packBOOTimg(char* fin1, char* fin2, char* fin3, char* fout){
	FILE *fi = NULL;
	FILE *fo = NULL;
	uint32_t i, size1,size2, size3, dsize, buffoff, result, chksize, chksum;
	
	uint32_t* buff1; //buffer for redboot
	uint32_t* buff2; //buffer for dram init 1
	uint32_t* buff3; //buffer for dram init 2
	
	dsize=0;//Total data size
	chksum=0;
		
	if ((fi = fopen(fin1, "rb")) == NULL) return -1;
	//Get first filesize
  fseek(fi , 0 , SEEK_END);
  size1 = ftell(fi);
  rewind(fi);
  dsize+=size1;
  //First buffer has 0x20 bytes for overall header + 0xC bytes for file header
  buff1 = (uint32_t*) malloc (size1+0x2C);
  //Set pointer offset to the start of the file header longword
	buffoff=8;
	//Make header
	//Stuff section magic word
	*(buff1+buffoff)=lswap(SECTION_MAGIC);
	buffoff+=1;
	//Stuff filesize
	*(buff1+buffoff)=size1;	
	buffoff+=1;
	//Stuff order indicator
	*(buff1+buffoff)=0x0;	
	buffoff+=1;		
	//Read data into buffer, leaving room for headers
	result = fread(&buff1[buffoff], 1, size1, fi);
	if (result != size1) return -2;
	fclose(fi);
	fi=NULL;
	//Do partial checksum
	if((size1/4) > 0x200) {
		chksize=0x200;
	}else{
		chksize = size1/4;
	}
	for(i=0xb; i<chksize+0xb; i++){
		chksum+=(*(buff1+i));
	}
	#ifdef DEBUG
		printf("Section 1 chk size is %0X is %0X\n",chksize, chksum);
	#endif
	
	
	//File 2
	if ((fi = fopen(fin2, "rb")) == NULL) return -3;
	//Get first filesize
  fseek(fi , 0 , SEEK_END);
  size2 = ftell(fi);
  rewind(fi);
  dsize+=size2;
  //Add 0xC bytes for file header
  buff2 = (uint32_t*) malloc (size2+0xC);
  //Set pointer offset to the start of the file header longword
	buffoff=0;
	//Make header
	//Stuff section magic word
	*(buff2+buffoff)=lswap(SECTION_MAGIC);
	buffoff+=1;
	//Stuff filesize
	*(buff2+buffoff)=size2;	
	buffoff+=1;
	//Stuff order indicator
	*(buff2+buffoff)=0x200;	
	buffoff+=1;		
	//Read data into buffer, leaving room for headers
	result = fread(&buff2[buffoff], 1, size2, fi);
	if (result != size2) return -4;
	fclose(fi);
	fi=NULL;
		//Do partial checksum
	if((size2/4) > 0x200) {
		chksize=0x200;
	}else{
		chksize = size2/4;
	}
	for(i=3; i<chksize+3; i++){
		chksum+=(*(buff2+i));
	}
	#ifdef DEBUG
		printf("Section 2 chk size is %0X is %0X\n",chksize, chksum);
	#endif
	
	//File 3
	if ((fi = fopen(fin3, "rb")) == NULL) return -5;
	//Get first filesize
  fseek(fi , 0 , SEEK_END);
  size3 = ftell(fi);
  rewind(fi);
  dsize+=size3;
  //Add 0xC bytes for file header
  buff3 = (uint32_t*) malloc (size3+0xC);
  //Set pointer offset to the start of the file header longword
	buffoff=0;
	//Make header
	//Stuff section magic word
	*(buff3+buffoff)=lswap(SECTION_MAGIC);
	buffoff+=1;
	//Stuff filesize
	*(buff3+buffoff)=size3;	
	buffoff+=1;
	//Stuff order indicator
	*(buff3+buffoff)=0x400;	
	buffoff+=1;		
	//Read data into buffer, leaving room for headers
	result = fread(&buff3[buffoff], 1, size3, fi);
	if (result != size3) return -6;
	fclose(fi);
	fi=NULL;
		//Do partial checksum
	if((size3/4) > 0x200) {
		chksize=0x200;
	}else{
		chksize = size3/4;
	}
	for(i=3; i<chksize+3; i++){
		chksum+=(*(buff3+i));
	}
	#ifdef DEBUG
		printf("Section 3 chk size is %0X is %0X\n",chksize, chksum);
	#endif
	
	buffoff=0;
	//Build main header
	*(buff1+buffoff)=lswap(BOOTFS_HEADMAGIC);
	buffoff+=1;
	*(buff1+buffoff)=size1+size2+size3 + 3*0xC; //Data length with section	 headers, no main header
	buffoff+=1;
	*(buff1+buffoff)=0x3;	//3 sections
	buffoff+=1;
	*(buff1+buffoff)=chksum;
	buffoff+=1;
	*(buff1+buffoff)=lswap(0x06000200);
	buffoff+=1;
	*(buff1+buffoff)=lswap(0x31313030);
	buffoff+=1;
	*(buff1+buffoff)=0;
	buffoff+=1;
	*(buff1+buffoff)=0;
		
	//Rearrange order of 2/3
	if ((fo = fopen(fout, "wb")) == NULL) return -7;
	fwrite(buff1, size1+0x2c, 1, fo);
	fwrite(buff2, size2+0xc, 1, fo);
	fwrite(buff3, size3+0xc, 1, fo);
	fclose(fo);
	
	free(buff1);
	free(buff2);
	free(buff3);
	
	printf("BOOT.IMG dumped and checksum'd\n");
	return 0;	
}

int main(int argc, char *argv[])
{
	uint32_t result;
	
  printf("========================================\n");
  printf("packboot - 2010, openschemes.com\n");
  printf("BOOT.IMG file packer\n");
  printf("========================================\n");
  
	if (argc < 5) {
		printf("Bad usage\n");
		printf("usage: packboot <BOOTLOADER> <IMG1> <IMG2> <OUTFILE>\n");
		printf("BOOTLOADER, IMG1, IMG2 are the files to pack\n");        
		printf("OUTFILE is the IMG file to write\n");     
		printf("Example: packboot RedBoot.mmp Init1.mmp Init2.mmp BOOT.IMG\n");  
		return -1;
	}

	printf("Packing %s %s %s into %s\n", argv[1], argv[2], argv[3], argv[4]);
	//Pack the files from BOOTFS dump into an IMG file for FRMPro
	result = packBOOTimg(argv[1], argv[2], argv[3], argv[4]);
	
	if(result != 0) printf("packboot failed with error code %d\n", result);
	
done:
	printf("Done!\n");
	return 0;
}
