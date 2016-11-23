//
// Created by marc on 15-11-16.
//

#include <iostream>
#include <cstring>
#include "Partition.h"
#include "../libfatfs/src/ff.h"
#include "../libfatfs/src/diskio.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    const char* path = "/tmp/marc.vault";

    // Create a partition
    char label[40] = "Marc";
    Partition partition = Partition::create(label, 1024, path);

    // Open an already created partition
    partition = Partition::open(path);

    currentFileDescriptor = partition.fd;

    // Workspace buffer
    BYTE work[_MAX_SS];

    const TCHAR* letter = (const TCHAR *) "";

    // Create a FAT16 filesytem
    std::cout << "fmkfs " << f_mkfs(letter, FM_FAT, 4096, work, sizeof work) << std::endl;


    // Filename
    const TCHAR* filename = (const TCHAR *) "sample.txt";

    FATFS FatFs;   // Filesystem

    f_mount(&FatFs, letter, 0);

    FIL fil;
    char line[82] = "Hello world";
    FRESULT fr;

    // Open file
    fr = f_open(&fil, filename, FA_CREATE_NEW | FA_WRITE);
    if (fr){
        std::cout << "could not open file" << std::endl;
        return fr;
    };

    // Write stuff
    UINT * writtenBytes = new UINT;
    std::cout << "fwrite " << f_write( &fil, line, sizeof line, writtenBytes) << std::endl;

    // Close file
    f_close(&fil);

    memset(line, 0, sizeof(line));



    f_open(&fil, filename, FA_READ);
    f_read(&fil, line, sizeof line, writtenBytes);

    std::cout << std::endl << "Read from file: " << std::endl << line;



    return 0;
}