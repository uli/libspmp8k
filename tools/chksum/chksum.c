#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned char *buffer;
    unsigned long fileLen, i;
    unsigned long myPtr, myStart, myStop, myLen;
    unsigned long myWord=0;
    unsigned long myFileChecksum=0;
    unsigned long myChecksum=0;

    if (argc < 3){
             printf("chksum.exe - Update SPMP8000 BOOT img file checksum\n");
             printf("Copyright 2010, Openschemes.com\n");
             printf("Usage: chksum <arg> <img file> <write?>\n");
             printf("Arguments: m = multi-segment image, s = single-segment image\n");
             printf("Write: Add w at the end to write corrected checksum back to file\n");
             printf("Example (no write): chksum m c:\\myimage.img\n");
             printf("Example (w/ write): chksum m c:\\myimage.img w\n");
    }
             
    fp = fopen(argv[2], "rb");
    
   	if (!fp)
	{
		printf("Unable to open file %s", argv[2]);
		return -1;
	}

	//Get file length
	fseek(fp, 0, SEEK_END);
	fileLen=ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	printf("Allocating %.2X bytes\n", fileLen);

	//Allocate memory
	buffer=(unsigned char *)malloc(fileLen+1);
	if (!buffer)
	{
		printf("Couldn't malloc!");
        fclose(fp);
		return -2;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, fp);
	fclose(fp);
	     
    if (argv[1][0]=='m')
    {
       printf("Multi-segment mode\n\n");

       //Set buffer offset
       myPtr=0x0c;
       //Translate & print current checksum
       myFileChecksum = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
       printf("Checksum Read from file is: %X\n\n", myFileChecksum);

       //SEGMENT 1 
       //Hard-code pointer to len header of segment 1
       myPtr=0x24;
       //Read segment 1 length from 0x24
       myStop = myLen = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
       printf("Segment 1 length header (at %X) is: %x\n",myPtr, myLen);
       
       //Set pointer to actual start of data and discuss details
       myStart=0x2c;
       printf("Segment 1 - Start:%x, Len:%x\n", myStart, myLen);
       if (myLen > 0x800) {
          myStop=0x800;
          printf("myLen > 0x800.  Using first 0x800 bytes only\n");
       }
    
       for (myPtr=myStart; myPtr<=myStart+myStop; myPtr+=4){
           myWord = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
           //printf("%X\n",myWord);
           myChecksum += myWord;
           }
       printf("Calculated checksum thru segment 1 is: %X\n\n", myChecksum);

       //SEGMENT 2 
       //Increase pointer by length of segment 1 + header(2) and get next segment len
       myStart+=myLen+4;
       //Read segment 2 length 
       myPtr=myStart;
       myStop = myLen = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
       printf("Segment 2 length header (at %X) is: %x\n",myPtr, myLen);

       //Add length of header to starting offset
       myStart+=0x08;
       printf("Segment 2 - Start:%x, Len:%x\n", myStart, myLen);
       if (myLen > 0x800) {
          myStop=0x800;
          printf("myLen > 0x800.  Using first 0x800 bytes only\n");
       }
    
       for (myPtr=myStart; myPtr<=myStart+myStop-1; myPtr+=4){
           myWord = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
           //printf("%X\n",myWord);
           myChecksum += myWord;
           }
       printf("Calculated checksum thru segment 2 is: %X\n\n", myChecksum);

       //SEGMENT 3 
       //Increase pointer by length of segment 1 + header(2) and get next segment len
       myStart+=myLen+4;
       //Read segment 2 length 
       myPtr=myStart;
       myStop = myLen = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
       printf("Segment 3 length header (at %X) is: %x\n",myPtr, myLen);

       //Add length of header to starting offset
       myStart+=0x08;
       printf("Segment 3 - Start:%x, Len:%x\n", myStart, myLen);
       if (myLen > 0x800) {
          myStop=0x800;
          printf("myLen > 0x800.  Using first 0x800 bytes only\n");
       }
    
       for (myPtr=myStart; myPtr<=myStart+myStop-1; myPtr+=4){
           myWord = (buffer[myPtr+3] << 24) + (buffer[myPtr+2] << 16) + (buffer[myPtr+1] << 8) + buffer[myPtr];
           //printf("%X\n",myWord);
           myChecksum += myWord;
           }
       printf("Calculated checksum thru segment 3 is: %X\n\n", myChecksum); 
       
       printf("File Checksum: %X, Calculated Checksum: %X\n", myFileChecksum, myChecksum);
       if (myFileChecksum == myChecksum) 
       {
          printf("MATCH!\n");
       } else 
       {   
          printf("NO MATCH!!!\n");
          if (argc>3) if (argv[3][0]=='w'){
             myPtr=0x0c;
             buffer[myPtr+3] = (myChecksum >> 24) & 0xFF;
             buffer[myPtr+2] = (myChecksum >> 16) & 0xFF;
             buffer[myPtr+1] = (myChecksum >> 8) & 0xFF;
             buffer[myPtr] = (myChecksum >> 0) & 0xFF;
             
             printf("Updating file to: %X, %X, %X, %X\n", buffer[myPtr], buffer[myPtr+1], buffer[myPtr+2], buffer[myPtr+3]);
             
             fp = fopen(argv[2], "wb");
   	         if (!fp)
	         {
		        printf("Unable to open file %s", argv[2]);
                return -1;
	         }
	         
	         fwrite(buffer, fileLen, 1, fp);
	         fclose(fp);
       
          }
        }//if (myFileChecksum == myChecksum)  
       printf("Done...\n");
    }//if (argv[1]="m") 
    
    free(buffer);
    return EXIT_SUCCESS;
}
