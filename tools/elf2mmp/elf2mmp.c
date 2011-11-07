#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	ELF_MAGIC		0x464C457F
#define	MMPI_MAGIC	0x69504d4d
#define	MMPD_MAGIC	0x64504d4d
#define	MMPS_MAGIC	0x73504d4d

#define DEBUG

typedef struct elf_fhdr{
	uint32_t	magic;
	uint8_t		dontcare1[20];
	uint32_t	entry;
	uint32_t	phoff;
	uint8_t		dontcare2[4];
	uint32_t	flags;
	uint16_t	hdsiz;
	uint16_t	phsiz;
	uint16_t	phnum;
	uint8_t		dontcare3[6];
} elf_fhdr_t;

typedef struct elf_phdr{
	uint32_t	type;
	uint32_t	foff;
	uint32_t	vadd;
	uint32_t	padd;
	uint32_t	fsiz;
	uint32_t	msiz;
	uint32_t	flag;
	uint32_t	algn;
} elf_phdr_t;
	

#define	ERR(msg) { \
	printf("error %s\n", msg); \
	goto done; \
}

int main(int argc, char *argv[])
{
	//declare ptrs
	FILE *f = NULL;
	FILE *fo = NULL;
	uint8_t* foutbuf;
	//declare variables
	uint32_t temp[8];
	uint32_t i, outlen, foutptr, stop;
	//declare structs
	elf_fhdr_t fhdr;
	
	//initialize variables
	temp[0]=0;
	temp[1]=1;
	
	printf("========================================\n");
  printf("elf2mmp - 2010, openschemes.com\n");
  printf("convert ELF file to SPMP8k \"MMP\" format\n");
  printf("========================================\n\n");
	
	if (argc < 3) {
		printf("Bad usage\n");
    printf("usage: elf2mmp <INFILE> <OUTFILE>\n");  
    return -1;
   }
		
	if ((f = fopen(argv[1], "rb")) == NULL) ERR("could not open input file")
		
	fread(&fhdr, sizeof(elf_fhdr_t), 1, f);
	
	#ifdef DEBUG
		printf("magic: 0x%08X\n", fhdr.magic);
	#endif
	
	if(fhdr.magic != ELF_MAGIC){
		 ERR("I don't see ELF magic here\n");
	}else{
		printf("Got an ELF File\n");
	}
	
	#ifdef DEBUG
		printf("entry: 0x%08X\n", fhdr.entry);
		printf("phoff: 0x%08X\n", fhdr.phoff);
		printf("flags: 0x%08X\n", fhdr.flags);
		printf("phsize: 0x%08X\n", fhdr.phsiz);
		printf("phnum: 0x%08X\n", fhdr.phnum);
		printf("phdr size: 0x%08X\n", sizeof(elf_phdr_t));
		printf("total size: 0x%08X\n", fhdr.phnum * sizeof(elf_phdr_t));
		printf("malloc'ing for phdrs\n");
	#endif
	
	//Get some memory for array of phdr structs
	elf_phdr_t* phdrs = (elf_phdr_t*) malloc(fhdr.phnum * sizeof(elf_phdr_t));
	#ifdef DEBUG
	       printf("mallocd\n");
	#endif 
	
	if(!phdrs) ERR("phdrs Malloc failed\n");
	
	//Get the phdr data
	fread(phdrs, (fhdr.phnum * sizeof(elf_phdr_t)), 1, f);

	#ifdef DEBUG
		printf("Headers read..\n");
	#endif
	
	outlen=0x10;  // Start by assiming there will be an MMPi header
	for(i=0; i < fhdr.phnum; i++){
		outlen += phdrs[i].fsiz + 0x18; // Add data length plus MMPs header length
		//if(phdrs[[i]) outlen += 16;
		#ifdef DEBUG
			printf("Header %d foff: 0x%08X\n", i, phdrs[i].foff);
			printf("Header %d vadd: 0x%08X\n", i, phdrs[i].vadd);
			printf("Header %d padd: 0x%08X\n", i, phdrs[i].padd);
			printf("Header %d fsiz: 0x%08X\n", i, phdrs[i].fsiz);
			printf("outlen: 0x%08X\n",outlen);
		#endif
	}
	outlen+=0x24;  //Add the length of the MMPd header to get the total length
	
	#ifdef DEBUG
		printf("Final Outlen: %0X.  Calling malloc for foutbuf\n",outlen);
	#endif

	//Allocate memory for building MMP file
	foutbuf=(uint8_t*)malloc(outlen);
	if (!foutbuf) ERR("Couldn't malloc\n")
	
	#ifdef DEBUG
		printf("Malloc'd fine, Building MMPs sections\n");
	#endif
		
	//Setup pointer in data buffer
	foutptr=0x34;		//Start of the MMPs sections
	
	//build the MMPs sections of the file
	for(i=0; i < fhdr.phnum; i++){
		//Store MMPs tag
		memcpy(&foutbuf[foutptr], "MMPs", 4);
		foutptr+=4;
		//Store segment size
		temp[2] = phdrs[i].fsiz + 0x18;	// Get length including header offset
		memcpy(&foutbuf[foutptr], &temp[2], 4);
		foutptr+=4;
		//Store physical address - double check phy first!?!?
		memcpy(&foutbuf[foutptr], &phdrs[i].padd, 4);
		foutptr+=4;
		//Store virtual address - double check phy first!?!?
		memcpy(&foutbuf[foutptr], &phdrs[i].vadd, 4);
		foutptr+=4;
		//Store constant 0
		memcpy(&foutbuf[foutptr], &temp[0], 4);
		foutptr+=4;
		//Store constant 1
		memcpy(&foutbuf[foutptr], &temp[1], 4);
		foutptr+=4;
		
		//Copy data from ELF to foutbuf
		fseek(f, phdrs[i].foff, SEEK_SET);
		//Read in data from file to foutbuf
		fread(&foutbuf[foutptr], phdrs[i].fsiz, 1, f);
		foutptr+=phdrs[i].fsiz;
	}
	
	#ifdef DEBUG
		printf("MMPs processing done, %0X bytes processed\n",foutptr);
		printf("Building MMPd section\n");
	#endif
	
	//Work in the MMPd section
	foutptr=0x10;
	temp[3]=0x00000024;
	temp[4]=0x01000001;
	temp[5]=0x61000001;  //We should REALLY or this with some other flags, but I forget which ones.  They're usually 0.
	temp[6]=0x07020207;
	memcpy(&foutbuf[foutptr], "MMPd", 4);//Stuff the MMPd header
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[3], 4);//Stuff 0x00000024 - constant length of header
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[4], 4);//Stuff 0x01000001
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[6], 4);//This should be a malformed system time, stuff a fake one for checksum
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[5], 4);//Stuff 0x61000001 - these flags should be or'd with something else.
	foutptr+=4;
	temp[7] = fhdr.phnum; // Lengthen # of segments to 32 bits
	memcpy(&foutbuf[foutptr], &temp[7], 4);//Stuff number of segments with size fixup
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &fhdr.entry, 4);//Stuff entry point
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[0], 4);	//Stuff 0
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[1], 4);	//Stuff 1
	foutptr+=4;
	
	#ifdef DEBUG
		printf("MMPd processing done, %0X bytes processed\n",foutptr-0x10);
		printf("Computing data checksum of MMPd and MMPs sections\n");
	#endif
	
	
	//Compute checksums in temp[6] and temp[7]
	temp[6]=0;
	temp[7]=0;
	foutptr=0x10;
	//Phase 1 - sum into chk1
	stop = foutptr + (((outlen-16)/16)*4);
	for(foutptr=foutptr; foutptr < stop; foutptr+=4){
		temp[6] += (foutbuf[foutptr+3] << 24) + (foutbuf[foutptr+2] << 16) + (foutbuf[foutptr+1] << 8) + foutbuf[foutptr+0];
	}
	#ifdef DEBUG
		printf("Checksum1 out of phase 1 is: 0x%08X\n", temp[6]);
	#endif
	
	//Phase 2 - sum into chk2
	stop = foutptr + (((outlen-16)/16)*4);
	for(foutptr=foutptr; foutptr < stop; foutptr+=4){
		temp[7] += (foutbuf[foutptr+3] << 24) + (foutbuf[foutptr+2] << 16) + (foutbuf[foutptr+1] << 8) + foutbuf[foutptr+0];
	}
	#ifdef DEBUG
		printf("Checksum2 out of phase 2 is: 0x%08X\n", temp[7]);
  #endif	
	
	//Phase 3 - sum into chk1
	stop = foutptr + (((outlen-16)/16)*4);
	for(foutptr=foutptr; foutptr < stop; foutptr+=4){
		temp[6] += (foutbuf[foutptr+3] << 24) + (foutbuf[foutptr+2] << 16) + (foutbuf[foutptr+1] << 8) + foutbuf[foutptr+0];
	}
	#ifdef DEBUG
		printf("Checksum1 out of phase 3 (final) is: 0x%08X\n", temp[6]);
	#endif
	
	//Phase 2 - sum into chk2
	stop = outlen;
	for(foutptr=foutptr; foutptr < stop; foutptr+=4){
		temp[7] += (foutbuf[foutptr+3] << 24) + (foutbuf[foutptr+2] << 16) + (foutbuf[foutptr+1] << 8) + foutbuf[foutptr+0];
	}
	#ifdef DEBUG
		printf("Checksum2 out of phase 4 (final) is: 0x%08X\n", temp[7]);
  #endif	
  
  #ifdef DEBUG
		printf("Checksum complete, final checksums: 0x%08X, 0x%08X\n",temp[6], temp[7]);
		printf("Building MMPi section\n");
	#endif
	
	//Reset pointer to 0, and finish up by doing the MMPi section
	foutptr=0;
	memcpy(&foutbuf[foutptr], "MMPi", 4);
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &outlen, 4);
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[6], 4);
	foutptr+=4;
	memcpy(&foutbuf[foutptr], &temp[7], 4);
	foutptr+=4;
	
	#ifdef DEBUG
		printf("MMP file complete!\n");
		printf("Opening %s for writing\n", argv[2]);
	#endif
	
	// Open the MMP file 
	if ((fo = fopen(argv[2], "wb")) == NULL) ERR("could not open output file")
	
	fwrite(foutbuf, outlen, 1, fo);
	fclose(fo);
	fo = NULL;

	#ifdef DEBUG
		printf("Wrote 0x%0X bytes to %s\n", outlen, argv[2]);
	#endif	
	
  done:
  if (foutbuf != NULL) free(foutbuf);
  if (phdrs != NULL) free(phdrs);
	if (f != NULL) fclose(f);
	if (fo != NULL) fclose(fo);
	printf("Done!\n");
	return 0;
}

