/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "ffconf.h"

/* Definitions of physical drive number for each drive */
#define DEV_PSV		0	/* Partition Safe Vault */
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		2	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		3	/* Example: Map USB MSD to physical drive 2 */


DWORD get_fattime(){
	return 0;
}

int reservedSectors = 10;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS result = STA_NOINIT;

	switch (pdrv) {
	    case DEV_PSV :
            result = 0x00;
            break;
	}
	return result;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS result = STA_NOINIT;

	switch (pdrv) {
        case DEV_PSV :
            result = 0x00;
            break;
	}
	return result;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    DRESULT result;

	switch (pdrv) {
        case DEV_PSV:

			fseek(currentFileDescriptor, _MAX_SS * sector + (reservedSectors * _MAX_SS), SEEK_SET);
			fread(buff, _MAX_SS * count, 1, currentFileDescriptor);
            break;
	}

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;
	DWORD startPosition = _MAX_SS * sector + (reservedSectors * _MAX_SS);

	switch (pdrv) {
        case DEV_PSV:
			fseek(currentFileDescriptor, startPosition, SEEK_SET);
			fwrite(buff, _MAX_SS * count, 1, currentFileDescriptor);
            break;
	}

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
        case DEV_PSV:
			switch (cmd){
				case GET_SECTOR_COUNT:
					*(DWORD*)buff =  10000;
					break;
				case GET_BLOCK_SIZE:
					*(DWORD*)buff = 1024;
					break;
			}
            break;

	}

	return RES_OK;
}

