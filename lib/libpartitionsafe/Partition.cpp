//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "Partition.h"
#include "common.cpp"

#if defined(__APPLE__) || defined(_WIN32)
#include <fcntl.h>
#else
#include <sys/file.h>
#endif

/**
 * Set the const identifier.
 *
 * NEVER, NEVER, NEVER CHANGE THIS VALUE.
 * WHO CHANGES THIS VALUE, WILL BE ASSASSINATED.
 */
const char* Partition::IDENTIFIER = "PartitionSafe";

/**
 * Set the current const version.
 */
const unsigned int Partition::VERSION = 1;

Partition::Partition(Header* header, const char* path, FILE* fh):
        header(header), path(path), fd(fh) {}

Partition Partition::open(const char* path) {
    // Open the file
    FILE* fd = fopen(path, "r+");

    // Try to get an exclusive lock
//    if(flock(fileno(fh), LOCK_EX) == 0){
//        std::cout << "All good"<< std::endl;
//    }else{
//        std::cout << "Could not acquire lock" << std::endl;
//    }

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, fd);

    // Check the identifier of the drive
    if(strcmp(header->identifier, Partition::IDENTIFIER) == 0){
        std::cout << "Partition: Good";
    }else{
        std::cout << "Partition: Bad, " << header->identifier;
    }

    // Print information
    std::cout<< std::endl;
    std::cout << "Label: " << header->label << std::endl;
    std::cout << "Size: " << header->size << std::endl;
    std::cout << "Version: " << header->version << std::endl;
    std::cout << "UUID: " << header->UUID << std::endl;

    // Open the partition and set information
    Partition partition = Partition(header, path);
    partition.fd = fd;

    return partition;
}

Partition Partition::create(char label[40], unsigned int size, const char* path) {
    FILE* fd = fopen(path, "w");

    // The partition header
    struct Header* header = new Header;
    strncpy(header->identifier, Partition::IDENTIFIER, sizeof(header->identifier));
    strncpy(header->label, label, sizeof(header->label));
    strncpy(header->UUID, "23456", sizeof(header->UUID));
    header->size = size;
    header->version = Partition::VERSION;

    // Write our partition header
    fwrite(&*header, 1, sizeof(*header), fd);

    // Make the partition file SIZE bytes long + header
    fseek(fd, (size - 1) + sizeof(*header) + 10, SEEK_SET);
    fputc('\0', fd);

    fclose(fd);

    return Partition(header, path);
}


