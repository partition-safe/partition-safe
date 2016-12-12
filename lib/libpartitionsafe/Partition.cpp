//
// Created by marc on 15-11-16.
//

#include "Partition.h"
#include "../libfatfs/src/diskio.h"
#include "Common.h"

/**
 * NEVER, NEVER, NEVER CHANGE THIS VALUE.
 * WHO CHANGES THIS VALUE, WILL BE ASSASSINATED.
 */
const unsigned char *Partition::IDENTIFIER = (const unsigned char *) "PartitionSafe";

/**
 * NEVER, NEVER, NEVER CHANGE THIS VALUE.
 * WHO CHANGES THIS VALUE, WILL BE ASSASSINATED.
 */
const TCHAR *Partition::LETTER = (const TCHAR *) "";

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
    if(res != FR_OK) throw "Could not mount partition";

    // Return myself
    return this;
}

Partition *Partition::create() {
    // Workspace buffer
    BYTE work[_MAX_SS];
    FRESULT res;

    // Make the file system
    res = f_mkfs(Partition::LETTER, FM_FAT, 4096, work, sizeof work);
    if(res != FR_OK) throw "Could not create file system";

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
    if (res != FR_OK) throw "Could not open file";

    // Write stuff
    res = f_write(&fil, buff, size, writtenBytes);
    if (res != FR_OK) throw "Could not write file";

    // Close the file
    res = f_close(&fil);
    if(res != FR_OK) throw "Could not close file";

    // Return myself
    return this;
}

Partition *Partition::writeFile(const std::string fileName, const void *buff, const UINT size) {
    return writeFile(Common::stdStringToTChar(fileName), buff, size);
}

#ifndef __WIN32
Partition *Partition::writeFile(const char *fileName, const void *buff, const UINT size) {
    return writeFile(std::string(fileName), buff, size);
}
#endif

Partition *Partition::fileInfo(const TCHAR *fileName, FILINFO *fileInfo) {
    // Instances
    FRESULT res;

    // Retrieve file status
    res = f_stat(fileName, fileInfo);
    if(res != FR_OK) throw "Could not get file status";

    // Return myself
    return this;
}

Partition *Partition::fileInfo(const std::string fileName, FILINFO *fileInfo) {
    return this->fileInfo(Common::stdStringToTChar(fileName), fileInfo);
}

#ifndef __WIN32
Partition *Partition::fileInfo(const char *fileName, FILINFO *fileInfo) {
    return this->fileInfo(std::string(fileName), fileInfo);
}
#endif

Partition *Partition::readFile(const TCHAR *fileName, void *buff, const UINT size) {
    // The instances
    FIL fil;
    UINT *readBytes = new UINT;
    FRESULT res;

    // Open the file
    res = f_open(&fil, fileName, FA_READ);
    if(res != FR_OK) throw "Could not open file";

    // Read file data
    res = f_read(&fil, buff, size, readBytes);
    if(res != FR_OK) throw "Could not read file";

    // Close the file
    res = f_close(&fil);
    if(res != FR_OK) throw "Could not close file";

    // Return myself
    return this;
}

Partition *Partition::readFile(const std::string fileName, void *buff, const UINT size) {
    return readFile(Common::stdStringToTChar(fileName), buff, size);
}

#ifndef __WIN32
Partition *Partition::readFile(const char *fileName, void *buff, const UINT size) {
    return readFile(std::string(fileName), buff, size);
}
#endif

std::vector<Entry*> *Partition::listDirectory(const TCHAR *directoryName) {
    // The instances
    FRESULT res;
    DIR dir;
    FILINFO fno;

    // Open the directory
    res = f_opendir(&dir, directoryName);
    if(res != FR_OK) throw "Could not open directory";

    // Convert directory name to string
    std::string sDirectoryName = Common::tCharToStdString(directoryName);

    // The result list
    std::vector<Entry*>* entries = new std::vector<Entry*>();

    // Find all directories
    for (;;) {
        // Read a directory item
        res = f_readdir(&dir, &fno);

        // Break on error or end of dir
        if (res != FR_OK || fno.fname[0] == 0) break;

        // Create the entry and add it to the list
        entries->push_back(new Entry(fno, sDirectoryName));
    }

    // Close the directory
    res = f_closedir(&dir);
    if(res != FR_OK) throw "Could not close directory";

    // Return myself
    return entries;
}

std::vector<Entry *> *Partition::listDirectory(const std::string directoryName) {
    return listDirectory(Common::stdStringToTChar(directoryName));
}

#ifndef __WIN32
std::vector<Entry *> *Partition::listDirectory(const char *directoryName) {
    return listDirectory(std::string(directoryName));
}
#endif

Partition *Partition::createDirectory(const TCHAR *directoryName) {
    // Instances
    FRESULT res;

    // Retrieve file status
    res = f_mkdir(directoryName);
    if(res != FR_OK) throw "Could not create directory";

    // Return myself
    return this;
}

Partition *Partition::createDirectory(const std::string directoryName) {
    return createDirectory(Common::stdStringToTChar(directoryName));
}

#ifndef __WIN32
Partition *Partition::createDirectory(const char *directoryName) {
    return createDirectory(std::string(directoryName));
}
#endif

Partition *Partition::deleteFileDirectory(const TCHAR *path) {
    // Instances
    FRESULT res;

    // Retrieve file status
    res = f_unlink(path);
    if(res != FR_OK) throw "Could not delete directory";

    // Return myself
    return this;
}

Partition *Partition::deleteFileDirectory(const std::string path) {
    return deleteFileDirectory(Common::stdStringToTChar(path));
}

#ifndef __WIN32
Partition *Partition::deleteFileDirectory(const char *path) {
    return deleteFileDirectory(std::string(path));
}
#endif

int Partition::importFile(const char *source, const char *destination) {
    int SIZE = 1024;

    // The instances
    FIL fDestination;
    UINT *writtenBytes = new UINT;
    FRESULT res;

    FILE* fSource = fopen(source, "r");

    if(fSource == NULL){
        throw "Could not open source file";
    }

    res = f_open(&fDestination, Common::stdStringToTChar(std::string(destination)), FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) throw "Could not open destination file";

    char buffer[SIZE];
    size_t bytes;

    while (0 < (bytes = fread(buffer, 1, sizeof(buffer), fSource))){
        res = f_write(&fDestination, buffer, bytes, writtenBytes);
        if (res != FR_OK) throw "Could not write import file";
    }

    fclose(fSource);
    f_close(&fDestination);
}

int Partition::exportFile(const char *source, const char *destination) {
    int SIZE = 1024;

    // The instances
    FIL fSource;
    UINT *bytesRead = new UINT;
    FRESULT res;

    res = f_open(&fSource, Common::stdStringToTChar(std::string(source)), FA_OPEN_ALWAYS | FA_READ);
    if (res != FR_OK) throw "Could not open source file";

    FILE* fDestination = fopen(destination, "w");
    if(fDestination == NULL){
        throw "Could not open destination file";
    }

    char buffer[SIZE];

    for(;;){
        f_read(&fSource, buffer, sizeof buffer, bytesRead);

        // Done reading so we can break out now
        if(*bytesRead == 0){
            break;
        }

        fwrite(buffer, 1, *bytesRead, fDestination);
    }

    fclose(fDestination);
    f_close(&fSource);
}

