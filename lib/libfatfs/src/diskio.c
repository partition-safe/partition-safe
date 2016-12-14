/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "ffconf.h"
#include "../../libmbedtls/include/mbedtls/aes.h"

/* Definitions of physical drive number for each drive */
#define DEV_PSV		0	/* Partition Safe Vault */

#define RESERVED_SECTORS 10
#define RESERVED_SECTORS_BYTES RESERVED_SECTORS * _MAX_SS

DWORD get_fattime(){
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS result;

	switch (pdrv) {
	    case DEV_PSV :
            result = 0x00;
            break;

		default:
			result = STA_NOINIT;
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
    DSTATUS result;

	switch (pdrv) {
        case DEV_PSV :
            result = 0x00;
            break;

		default:
			result = STA_NOINIT;
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
    BYTE outBuf[_MAX_SS * count];

    switch (pdrv) {
        case DEV_PSV:
            // Read data
            fseek(currentFileDescriptor, _MAX_SS * sector + RESERVED_SECTORS_BYTES, SEEK_SET);
            fread(outBuf, _MAX_SS * count, 1, currentFileDescriptor);

            // Setup decrypt variables
            unsigned char key[32] = "lol";
            unsigned char iv[16] = "lol";
            mbedtls_aes_context dtx;

            // Initialize AES
            mbedtls_aes_init(&dtx);

            // Set decryption key
            mbedtls_aes_setkey_dec(&dtx, key, 256);
            if(mbedtls_aes_crypt_cbc(&dtx, MBEDTLS_AES_DECRYPT, _MAX_SS * count, iv, outBuf, buff) == 0) {
                result = RES_OK;
            } else {
                result = RES_ERROR;
            }

            // Cleanup
            mbedtls_aes_free(&dtx);
            break;

        default:
            result = RES_ERROR;
            break;
    }

    return result;
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
    DRESULT result = RES_OK;
    DWORD startPosition = _MAX_SS * sector + RESERVED_SECTORS_BYTES;
    BYTE outBuf[_MAX_SS * count];

    // Setup encryption variables
    unsigned char key[32] = "lol";
    unsigned char iv[16] = "lol";
    mbedtls_aes_context ctx;

    switch (pdrv) {
        case DEV_PSV:
            // Initialize AES
            mbedtls_aes_init(&ctx);

            // Set encryption key
            mbedtls_aes_setkey_enc(&ctx, key, 256);

            // Encrypt buffer
            if(mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, _MAX_SS * count, iv, buff, outBuf) != 0) {
                result = RES_ERROR;
            }

            // Cleanup
            mbedtls_aes_free(&ctx);

            // Result already error?
            if(result == RES_ERROR) {
                break;
            }

            // Write the result
            fseek(currentFileDescriptor, startPosition, SEEK_SET);
            fwrite(outBuf, _MAX_SS * count, 1, currentFileDescriptor);
            result = RES_OK;
            break;

        default:
            result = RES_ERROR;
            break;
    }
    return result;
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
	DRESULT result;

	switch (pdrv) {
        case DEV_PSV:
			// Pre-set the result
			result = RES_OK;

			// Switch for the command
			switch (cmd) {
				case GET_SECTOR_COUNT:
					*(DWORD*)buff =  500000;
					break;

				case GET_BLOCK_SIZE:
					*(DWORD*)buff = 1024;
					break;

				default:
					break;
			}
            break;

		default:
			result = RES_ERROR;
			break;
	}

	return result;
}

