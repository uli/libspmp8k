
/*
 * tool to upload & execute code via usb while in boot rom mode
 * (for spmp8000 based devices)
 * 
 * 2010 alemaxx + credits to openschemes.com
 * 
 * compile: gcc -lusb romboy.c -o romboy
 * (uses libusb)
 * 
 * run: sudo ./romboy -x test.mmp
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <usb.h>


#define	USB_TIMEOUT	512

#define	VENDORID	0x4fc
#define	PRODUCTID	0x7201

// commands:
//  0x0a, 0x15, 0x2e	- [0x9D80006C] = 1 (actually everything except 0xc2)
//	0x2b, 0x2c
//	

#define	SCSI_C2_CMD(n)	((0xc2000000) | ((n & 0xff) << 16))

// 00, 01, 02, 05, 06, 07, 10, 11, 12, 13
#define	SCSI_C2_BYPASS		SCSI_C2_CMD(0x00)	// resumes normal boot procedure (i.e. flash-boot)
#define	SCSI_C2_WRMEM		SCSI_C2_CMD(0x01)	// upload data to device memory
#define	SCSI_C2_RDMEM		SCSI_C2_CMD(0x02)	// download device memory
#define	SCSI_C2_UNK00		SCSI_C2_CMD(0x03)	// nothing
#define	SCSI_C2_UNK01		SCSI_C2_CMD(0x04)	// nothing
#define	SCSI_C2_EXEC		SCSI_C2_CMD(0x05)	// executes code at a given location in memory
#define	SCSI_C2_VERSION		SCSI_C2_CMD(0x06)	// prints "+++MMP RomCode ver 0.3.0 2008/06/05"
#define	SCSI_C2_INITRAM		SCSI_C2_CMD(0x07)	// calls "InitDRAM2" / load dram init code from flash and executes it

// NAND stuff (???)
#define	SCSI_C2_UNK10		SCSI_C2_CMD(0x10)
#define	SCSI_C2_BLKEARSE	SCSI_C2_CMD(0x11)
#define	SCSI_C2_WRPAGE		SCSI_C2_CMD(0x12)
#define	SCSI_C2_RDPAGE		SCSI_C2_CMD(0x13)

#define LSWAP(lw)	(((lw&0xFF)<<24) + ((lw&0xFF00)<<8) + ((lw&0xFF0000)>>8) + ((lw&0xFF000000)>>24))

#define	CODE_ADDR			0x200000			// make it a parameter

// usbmassbulk_10.pdf
typedef struct cbw {
	uint32_t   sig;			// 0
	uint32_t   tag;			// 4
	uint32_t   xlen;		// 8
	uint8_t    flag;		// 12
	uint8_t    lun;			// 13
	uint8_t    blen;		// 14
	uint32_t   cmd;			// 15
	uint32_t   adr;
	uint32_t   unk[2];
}__attribute__((packed)) cbw_t;

typedef struct csw {
	uint32_t   sig;			// 0
	uint32_t   tag;			// 4
	uint32_t   residue;		// 8
	uint8_t    status;		// 12
}__attribute__((packed)) csw_t;

// forward declarations
usb_dev_handle *open_device(uint16_t vid, uint16_t pid);
void dumphex(uint8_t *buff, int length);
void usage(char *myname);
int load_file(char *filename, char **data);

int get_csw(usb_dev_handle *hdev, csw_t *pcsw);
void print_csw(csw_t *pcsw);

// ROM commands
int usb_version(usb_dev_handle *hdev);
int usb_bypass(usb_dev_handle *hdev);
int usb_dram_init(usb_dev_handle *hdev);
int usb_exec(usb_dev_handle *hdev, uint32_t addr);
int usb_wr_mem(usb_dev_handle *hdev, uint32_t addr, uint32_t length, char *data);
int usb_rd_mem(usb_dev_handle *hdev, uint32_t addr, uint32_t length, char *data);
int usb_rd_flashpg(usb_dev_handle *hdev, uint32_t pg, uint8_t *data);
int usb_wr_flashpg(usb_dev_handle *hdev, uint32_t pg, uint8_t *data);
int usb_erase_flashblk(usb_dev_handle *hdev, uint32_t block);

int test_some_cmd(usb_dev_handle *hdev);

uint32_t bytes_per_page;
uint32_t pages_per_block;
uint32_t total_blocks;

// main
int main(int argc, char **argv)
{
	usb_dev_handle *hdev = NULL;
	uint32_t resp = 0;
	uint32_t conf[4];
	char *mmp = NULL;
	cbw_t cbw;
	int ret = 0;
	
//	printf("--==BANNER==--\n");
	printf("romboy 6.6.6\n");
	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}

	// initialize libusb
	usb_init();

	// configure device
	printf("initializing device...");
	hdev = open_device(VENDORID, PRODUCTID);
	if (hdev == NULL) {
		printf("error: could not find device\n");
		ret = -2;
		goto finish;
	}

	usb_reset(hdev);

	if (usb_detach_kernel_driver_np(hdev, 0)) printf("\ndetached..\n");

	ret = usb_set_configuration(hdev, 1);
	if (ret < 0) {
		printf("error: could not select configuration (1)\n");
		ret = -3;
		goto finish;
	}

	ret = usb_claim_interface(hdev, 0);
	if (ret < 0) {
		printf("error: could not claim interface #0\n");
		ret = -4;
		goto finish;
	}
	printf("done\n");
	
	// gather device info
	ret = usb_rd_mem(hdev, 0x9d805820, 16, (char*)&conf[0]);
	bytes_per_page  = conf[1] & 0xFFFF;
	pages_per_block = conf[0] & 0xFFFF;
	total_blocks    = conf[2] >> 16;

// ================================ MAIN ================================
	if (argc > 1) {
		if (argv[1][0] == '-') {
			switch (argv[1][1]) {
				case 'i':
				{	
					printf("Flash Info:\n");
					printf("bytes per page  : %d\n", bytes_per_page); 
					printf("pages per block : %d\n", pages_per_block);
					printf("# blocks total  : %d\n", total_blocks);
					printf("total size      : %uMB\n", (total_blocks * pages_per_block * bytes_per_page) >> 20);
					
					break;
				}
			/*
				case 'a':
				{
					test_some_cmd(hdev);
					break;
				}
			*/
				case 'x':	// execute code
				{
					int sz;
					
					if (argc < 3) {
						printf("error: to few parameters\n");
						usage(argv[0]);
						goto finish;
					}
					
					printf("loading \"%s\"\n", argv[2]);
					sz = load_file(argv[2], &mmp);
					if (sz < 0) {
						ret = sz;
						printf("error (%d)\n", ret);
						goto finish;
					}
					else printf(" %d bytes loaded\n", sz);
					
					printf("initializing DRAM...");
					ret = usb_dram_init(hdev);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					
					printf("uploading code...");
					ret = usb_wr_mem(hdev, CODE_ADDR, (sz + 0x1ff) & 0xfffffe00, mmp);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					
					printf("executing code...");
					usb_exec(hdev, CODE_ADDR);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					break;
				}
					
				case 'r':	// read from memory
				{
					char buff[512];
					uint32_t addr;
					
					if (argc < 3) {
						printf("error: to few parameters\n");
						usage(argv[0]);
						goto finish;
					}
					
					printf("initializing DRAM...");
					ret = usb_dram_init(hdev);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					
					addr = strtoul(argv[2], NULL, 16);
					
					printf("read from 0x%x...", addr);
					fflush(stdout);
					ret = usb_rd_mem(hdev, addr, 512, buff);
					if (ret < 0) printf(" failed\n");
					else {
						printf("done");
						dumphex(&buff[0], 0x100);
					}
					break;
				}
					
				case 'w':	// write to memory
				{
					int sz;
				
					if (argc < 3) {
						printf("error: to few parameters\n");
						usage(argv[0]);
						goto finish;
					}
					
					printf("loading \"%s\"\n", argv[2]);
					sz = load_file(argv[2], &mmp);
					if (sz < 0) {
						ret = sz;
						printf("error (%d)\n", ret);
						goto finish;
					}
					else printf(" %d bytes loaded\n", sz);
					
					printf("initializing DRAM...");
					ret = usb_dram_init(hdev);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					
					printf("uploading %d bytes of data...", sz);
					ret = usb_wr_mem(hdev, CODE_ADDR, (sz + 0x1ff) & 0xfffffe00, mmp);
					if (ret < 0) {
						printf(" failed (%d)\n", ret);
						goto finish;
					}
					printf(" done\n");
					
					break;
				}
				
				case 'f':
					// dump flash
					if ((argv[1][2] == 'r') && (argc == 5)) {
						uint32_t start, len, n;
						uint8_t pg[4096];
						FILE *f = NULL;
						
						start = strtoul(argv[2], 0, 16);
						len   = strtoul(argv[3], 0, 16);
						
						if (len < 1) goto finish;
						
						if ((f = fopen(argv[4], "w+")) == NULL) {
							printf("error: cannot open file \"%s\"\n", argv[4]);
							goto finish;
						}
						
						printf("dumping flash pages 0x%x to 0x%x...\n", start, start+len);
						
						printf("%08X", start);
						fflush(stdout);
						for (n=start; n<(start + len); n++) {
							usb_rd_flashpg(hdev, n, pg);
							fwrite(pg, 4096, 1, f);
							if ((n & 15) == 0) {
								printf("\b\b\b\b\b\b\b\b%08X", n);
								fflush(stdout);
							}
						}
						printf("\b\b\b\b\b\b\b\b%08X - done\n", n);
						fclose(f);
					}
					else if (argv[1][2] == 'w') {
						uint32_t addr, len;
						char *data;
						int i;
						
						addr = strtol(argv[3], 0, 16);
						printf("write file %s to pages starting from %d\n", argv[2], addr);
						
						len = load_file(argv[2], &data);
						
						printf("filesize: %d bytes (%d pages)\n", len, (len + (bytes_per_page - 1)) / bytes_per_page);
						
						for (i=0; i<((len + (bytes_per_page - 1)) / bytes_per_page); i++) {
							usb_wr_flashpg(hdev, addr + i, &data[i * bytes_per_page]);
						}
						
						free(data);
					/*
						char pgdata[0x1100], pgdata2[0x1000];
						int ret;

						memset(pgdata, 0xaa, 0x1000);

						printf("flash write test\n");
						ret = usb_wr_flashpg(hdev, 0x20001, pgdata);

						ret = usb_rd_flashpg(hdev, 0x20001, pgdata2);
						ret = memcmp(pgdata, pgdata2, 0x1000);
						if (ret == 0) printf("write success!!!\n");
						else {
							printf("write failed:\n");
							dumphex(pgdata2, 0x80);
						}
					*/
					}
					else if (argv[1][2] == 'e') {
						uint32_t addr;
						
						addr = strtol(argv[2], 0, 16);
						printf("ERASING BLOCK %d!!!\n", addr);
						usb_erase_flashblk(hdev, addr);	// seems to be page based address
					}
					break;
					
				case 'b':	// resume boot from flash
					printf("start normal boot procedure...");
					usb_bypass(hdev);
					if (ret < 0) printf(" error (%d)\n", ret);
					else printf("done\n");
					break;
					
				case 't':	// print ROM version (via serial)
					printf("print ROM version...");
					ret = usb_version(hdev);
					if (ret < 0) printf("cmd_version error (%d)\n", ret);
					else printf("done\n");
					break;
				
				case 'd':	// init dram
					printf("initializing DRAM...");
					ret = usb_dram_init(hdev);
					if (ret < 0) printf(" failed\n");
					else printf(" done\n");
					break;
			}
		}
	}

// ======================================================================

finish:
	if (mmp != NULL) free(mmp);
	if (hdev != NULL) usb_close(hdev);
	if (ret < 0) printf("[error] %d\n", ret);

	return ret;
}


usb_dev_handle *open_device(uint16_t vid, uint16_t pid)
{
	struct usb_bus *pbus;
	struct usb_device *pdev;

	usb_find_busses();
	usb_find_devices();

	for(pbus = usb_get_busses(); pbus; pbus = pbus->next) {
		for(pdev = pbus->devices; pdev; pdev = pdev->next) {
			if ((pdev->descriptor.idVendor == vid)
				&& (pdev->descriptor.idProduct == pid)) {
				return usb_open(pdev);
			}
		}
	}

	return NULL;
}

int get_csw(usb_dev_handle *hdev, csw_t *pcsw)
{
	int ret;

	ret = usb_bulk_read(hdev, 0x81, (char*)pcsw, sizeof(csw_t), USB_TIMEOUT);
	if (ret < sizeof(csw_t)) {
		printf("CSW error\n");
		return ret;
	}

	if (pcsw->sig != 0x53425355) {
		printf("csw: invalid signature!\n");
		return -1;
	}

	return 0;
}

void print_csw(csw_t *pcsw)
{
	if (pcsw->sig == 0x53425355)
		printf("sig     : %c%c%c%c\n", pcsw->sig & 0xff, (pcsw->sig >> 8) & 0xff, (pcsw->sig >> 16) & 0xff, (pcsw->sig >> 24) & 0xff);
	else printf("sig     : %08x\n", pcsw->sig);
	printf("tag     : %08x\n", pcsw->tag);
	printf("residue : %08x\n", pcsw->residue);
	printf("status  : %d\n", pcsw->status);
}

// ------------------- ROM FUNCTIONS -----------------------------------

int usb_version(usb_dev_handle *hdev)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;

	cbw.sig    = LSWAP(0x55534243); //USBC
	cbw.tag    = 0;
	cbw.xlen   = 0;
	cbw.flag   = 0x80;
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_VERSION);
	cbw.adr    = 0;
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;

	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT);
	if (ret < 0) return ret;

	get_csw(hdev, &csw);
//	print_csw(&csw);

	return 0;
}

int usb_bypass(usb_dev_handle *hdev)
{
	cbw_t cbw;
	csw_t csw;

	cbw.sig    = LSWAP(0x55534243); //USBC
	cbw.tag    = 0;
	cbw.xlen   = 0;
	cbw.flag   = 0x80;
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(0xC2000000);
	cbw.adr    = 0;
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;

	return usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT);
}

int usb_dram_init(usb_dev_handle *hdev)
{
	cbw_t cbw;
	csw_t csw;
	int ret;

	cbw.sig    = LSWAP(0x55534243); //USBC
	cbw.tag    = 0;
	cbw.xlen   = 0;
	cbw.flag   = 0x80;
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_INITRAM);
	cbw.adr    = 0;
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;

	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT);

	get_csw(hdev, &csw);
//	print_csw(&csw);

	return ret;
}

int usb_exec(usb_dev_handle *hdev, uint32_t addr)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;

	cbw.sig    = LSWAP(0x55534243); //USBC
	cbw.tag    = 0;
	cbw.xlen   = 0;
	cbw.flag   = 0x80;
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x05));
	cbw.adr    = LSWAP(addr);
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;

	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT);
	if (ret < 0) return ret;

	get_csw(hdev, &csw);

	return 0;
}

int usb_wr_mem(usb_dev_handle *hdev, uint32_t addr, uint32_t length, char *data)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	int len = length;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = LSWAP(0x87654320); // transfer ID - relates cbw and csw
	cbw.xlen   = len;
	cbw.flag   = 0x00;  // host -> device
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x01));
	cbw.adr    = LSWAP(addr);
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;

	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
	ret = usb_bulk_write(hdev, 0x02, &data[0], len, USB_TIMEOUT*10);

	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}

	return 0;
}

int usb_rd_mem(usb_dev_handle *hdev, uint32_t addr, uint32_t length, char *data)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	int len = length;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = LSWAP(0x87654321); // transfer ID - relates cbw and csw
	cbw.xlen   = len;
	cbw.flag   = 0x80;	// device -> host
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x02));
	cbw.adr    = LSWAP(addr);
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;
	
	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
	ret = usb_bulk_read(hdev, 0x01, &data[0], len, USB_TIMEOUT*10);
	
	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}
	
	return 0;
}

int usb_rd_flashpg(usb_dev_handle *hdev, uint32_t pg, uint8_t *data)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = LSWAP(0x87654321); // transfer ID - relates cbw and csw
	cbw.xlen   = 0x1000;			// 4kb page
	cbw.flag   = 0x80;				// device -> host
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x13));
	cbw.adr    = LSWAP(pg);
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;
	
	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
	ret = usb_bulk_read(hdev, 0x01, &data[0], 0x1000, USB_TIMEOUT*10);
	
	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}
	
	return ret;
}

// nand commands:
//
//	0x00 0x30	page read
//	0x31 x		page read cache mode start
//	0x3f x		page read cache mode start last
//	...
//	0x90 x		read id
//	0x70 x		read status
//	0x80 0x10	program page
//	0x80 0x15	program page cache mode
//	0x60 0xd0	erase block
//	0xff x		reset
//
int usb_wr_flashpg(usb_dev_handle *hdev, uint32_t pg, uint8_t *data)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	int length = bytes_per_page;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = 0;
	cbw.xlen   = length;			// 4kb page
	cbw.flag   = 0x00;				// host -> device
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x12));
	cbw.adr    = LSWAP(pg);
	cbw.unk[0] = 0;//
	cbw.unk[1] = 0;

//	dumphex(data, 0x20);
	
//	printf("sending write_page(0x%x) command...", pg);
	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
//	printf("done (%d)\n", ret);
	

//	printf("sending data...");
	ret = usb_bulk_write(hdev, 0x02, data, length, USB_TIMEOUT*10);
//	printf("done (%d)\n", ret);

	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}
//	print_csw(&csw);

	return ret;
}

int usb_erase_flashblk(usb_dev_handle *hdev, uint32_t block)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	int length = 0x1000;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = 0;
	cbw.xlen   = length;			// 4kb page
	cbw.flag   = 0x00;				// host -> device
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(SCSI_C2_CMD(0x11));
	cbw.adr    = LSWAP(block);
	cbw.unk[0] = 0;
	cbw.unk[1] = 0;
	
//	printf("sending erase_block(0x%x) command...", block);
	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
//	printf("done (%d)\n", ret);

	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}

	print_csw(&csw);

	return ret;
}
/*
int test_some_cmd(usb_dev_handle *hdev)
{
	cbw_t cbw;
	csw_t csw;
	int ret = 0;
	int length = 0x400;
	
	cbw.sig    = LSWAP(0x55534243);
	cbw.tag    = LSWAP(0x78563412); // transfer ID - relates cbw and csw
	cbw.xlen   = length;			// 4kb page
	cbw.flag   = 0x00;				// host -> device
	cbw.lun    = 0;
	cbw.blen   = 0xA;
	cbw.cmd    = LSWAP(0x23000000);
	cbw.adr    = 9;
	cbw.unk[0] = 10;
	cbw.unk[1] = 11;
	
	printf("sending write_page command...");
	ret = usb_bulk_write(hdev, 0x02, (char*)&cbw, sizeof(cbw_t), USB_TIMEOUT*10);
	printf("done (%d)\n", ret);

	usleep(10000);
	if (get_csw(hdev, &csw) < 0) {
		print_csw(&csw);
		return -1;
	}
	print_csw(&csw);
 }
*/
// ------------------- THE LITTLE HELPERS ------------------------------

// cbw members
// 0x24		xlen (32bit)
// 0x48		flag (8bit)
// 0x4c		lun  (4bit)
// 0x50		blen (8bit)

void dumphex(uint8_t *buff, int length)
{
	int i, j;
	
	for (i=0; i<length; i++) {
		if ((i & 15) == 0) printf("\n%08X : ", i);
		printf("%02X ", buff[i]);
		if ((i & 15) == 15) {
			printf(" - ");
			for (j=(i-15); j<i; j++) {
				if ((buff[j] > 0x1f) && (buff[j] < 0x7f)) printf("%c", buff[j]);
				else printf(".");
			}
		}
	}
	printf("\n");
}

int load_file(char *filename, char **data)
{
	FILE *f;
	long sz, len;

	f = fopen(filename, "rb");
	if (f == NULL) {
		printf("[load_file] cannot open file \"%s\"\n", filename);
		return -1;
	}

	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	len = sz + (bytes_per_page - 1);
	len = len / bytes_per_page;
	len = len * bytes_per_page;
	
	if (sz != len) printf("padding file data %d -> %d\n", sz, len);

	*data = (char*)malloc(len);
	if (*data == NULL) {
		printf("[load_file] failed to allocate %d bytes\n", len);
		fclose(f);
		return -2;
	}
	
	printf("clear memory..."); fflush(stdout);
	memset(*data, 0xff, len);
	printf("done\n");
	
//	if (*data == NULL) return -2;

	if (fread(*data, 1, sz, f) != sz) {
		printf("[load_file] read error\n");
		fclose(f);
		free(*data);
		return -3;
	}

	fclose(f);
	return sz;
}

void usage(char *myname)
{
	printf("usage: %s -[ixrwbtdf(rwe)] ...\n\n", myname);
	printf("  i                      - print some info\n");
	printf("  b                      - resume booting from NAND\n");
	printf("  d                      - load and execute DRAM init from NAND\n");
	printf("  t                      - print ROM version to serial\n");
	printf("  x mmp_file             - execute MMP file\n");
	printf("  r 0x..                 - read from memory (SRAM/DRAM)\n");
	printf("  w filename 0x..        - write to memory (SRAM/DRAM)\n");
	printf("  fr filename start len  - dump NAND to file\n");
	printf("  fw filename start      - write data to NAND (erase first manually!)\n");
	printf("  fe start-page          - erases the block start_page belongs to\n");
	printf("\n\n");
}
