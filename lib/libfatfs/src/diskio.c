/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <memory.h>
#include "diskio.h"		/* FatFs lower layer API */
#include "ffconf.h"
#include "../../libmbedtls/include/mbedtls/aes.h"

/* Definitions of physical drive number for each drive */
#define DEV_PSV		0	/* Partition Safe Vault */

#define RESERVED_SECTORS 10
#define RESERVED_SECTORS_BYTES RESERVED_SECTORS * _MAX_SS

struct DISK_ENCRYPTION_CONFIG _disk_encryption_conf = {};
FILE *currentFileDescriptor;

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

            // Decrypt the data
            result = _disk_read_decrypt(_MAX_SS * count, outBuf, buff);
            break;

        default:
            result = RES_ERROR;
            break;
    }

    return result;
}

DRESULT _disk_read_decrypt(int size, BYTE *encryptedBuf, BYTE *decryptedBuf) {
    // Setup decrypt variables
    DRESULT result;
    unsigned char key[32];
    unsigned char iv[16];
    memcpy(key, _disk_encryption_conf.key, 32);
    memcpy(iv, _disk_encryption_conf.iv, 16);
    mbedtls_aes_context dtx;

    // Initialize AES
    mbedtls_aes_init(&dtx);

    // Set decryption key
    mbedtls_aes_setkey_dec(&dtx, key, 256);
    int res = mbedtls_aes_crypt_cbc(&dtx, MBEDTLS_AES_DECRYPT, size, iv, encryptedBuf, decryptedBuf);
    result = (res == 0 ? RES_OK : RES_DECERR);

    // Cleanup
    mbedtls_aes_free(&dtx);
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
    DRESULT result;
    DWORD startPosition = _MAX_SS * sector + RESERVED_SECTORS_BYTES;
    BYTE outBuf[_MAX_SS * count];
    DRESULT res;

    switch (pdrv) {
        case DEV_PSV:
            // Encrypt data
            result = _disk_write_encrypt(_MAX_SS * count, buff, outBuf);
            if(result == RES_OK) {
                // Write new buffer
                fseek(currentFileDescriptor, startPosition, SEEK_SET);
                fwrite(outBuf, _MAX_SS * count, 1, currentFileDescriptor);
            }
            break;

        default:
            result = RES_ERROR;
            break;
    }
    return result;
}


DRESULT _disk_write_encrypt(int size, BYTE const *unEncruptedBuf, BYTE *encryptedBuf) {
    // Setup encryption variables
    DRESULT result;
    unsigned char key[32];
    unsigned char iv[16];
    memcpy(key, _disk_encryption_conf.key, 32);
    memcpy(iv, _disk_encryption_conf.iv, 16);
    mbedtls_aes_context ctx;

    // Initialize AES
    mbedtls_aes_init(&ctx);

    // Set encryption key
    mbedtls_aes_setkey_enc(&ctx, key, 256);

    // Encrypt buffer
    int res = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, size, iv, unEncruptedBuf, encryptedBuf);
    result = (res == 0 ? RES_OK : RES_ENCERR);

    // Cleanup
    mbedtls_aes_free(&ctx);

    // Result already error?
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

