//
// Created by marc on 15-11-16.
//

#include "Partition.h"
#include "../libfatfs/src/diskio.h"

/**
 * NEVER, NEVER, NEVER CHANGE THIS VALUE.
 * WHO CHANGES THIS VALUE, WILL BE ASSASSINATED.
 */
const char* Partition::IDENTIFIER = "PartitionSafe";

/**
 * NEVER, NEVER, NEVER CHANGE THIS VALUE.
 * WHO CHANGES THIS VALUE, WILL BE ASSASSINATED.
 */
const TCHAR* Partition::LETTER = (const TCHAR *) "";

const unsigned int Partition::VERSION = 1;

Partition::Partition(const char* path, FILE* fh):
        path(path), fd(fh) {
    // Set the global fd
    currentFileDescriptor = fd;
}

Partition *Partition::open() {
    // The instances
    FRESULT res;

    // Mount the file system
    res = f_mount(&fs, Partition::LETTER, 0);
    if(res > FR_OK) throw "Could not mount partition";

    // Return myself
    return this;
}

Partition *Partition::create() {
    // Workspace buffer
    BYTE work[_MAX_SS];

    // Make the file system
    if(f_mkfs(Partition::LETTER, FM_FAT, 4096, work, sizeof work)) throw "Could not create file system";

    // Return myself
    return this;
}

Partition *Partition::writeFile(const TCHAR *fileName, const void *buff, const UINT size) {
    // The instances
    FIL fil;
    UINT *writtenBytes = new UINT;
    FRESULT res;

    // Open file
    res = f_open(&fil, fileName, FA_CREATE_NEW | FA_WRITE);
    if (res > FR_OK) throw "Could not open file";

    // Write stuff
    res = f_write(&fil, buff, size, writtenBytes);
    if (res > FR_OK) throw "Could not write file";

    // Close the file
    res = f_close(&fil);
    if(res > FR_OK) throw "Could not close file";

    // Return myself
    return this;
}

Partition *Partition::fileInfo(const TCHAR *fileName, FILINFO *fileInfo) {
    // Instances
    FRESULT res;

    // Retrieve file status
    res = f_stat(fileName, fileInfo);
    if(res > FR_OK) throw "Could not get file status";

    // Return myself
    return this;
}

Partition *Partition::readFile(const TCHAR *fileName, void *buff, const UINT size) {
    // The instances
    FIL fil;
    UINT *readBytes = new UINT;
    FRESULT res;

    // Open the file
    res = f_open(&fil, fileName, FA_READ);
    if(res > FR_OK) throw "Could not open file";

    // Read file data
    res = f_read(&fil, buff, size, readBytes);
    if(res > FR_OK) throw "Could not read file";

    // Close the file
    res = f_close(&fil);
    if(res > FR_OK) throw "Could not close file";

    // Return myself
    return this;
}
