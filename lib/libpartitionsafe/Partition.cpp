//
// Created by marc on 15-11-16.
//

#include <cstring>
#include <sys/file.h>
#include "Partition.h"
#include "common.cpp"

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
        header(header), path(path), fh(fh) {}

Partition Partition::open(const char* path) {
    // Open the file
    FILE* fh = fopen(path, "r+");

    // Is the file locked?
    if(flock(fileno(fh), LOCK_EX) == 0){
        std::cout << "All good"<< std::endl;
    }else{
        std::cout << "Could not acquire lock" << std::endl;
    }

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, fh);

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
    partition.fh = fh;

    return partition;
}

Partition Partition::create(char label[40], unsigned int size, const char* path) {
    FILE* fh = fopen(path, "w");

    // The partition header
    struct Header* header = new Header;
    strncpy(header->identifier, Partition::IDENTIFIER, sizeof(header->identifier));
    strncpy(header->label, label, sizeof(header->label));
    strncpy(header->UUID, newUUID().c_str(), sizeof(header->UUID));
    header->size = size;
    header->version = Partition::VERSION;

    // Write our partition header
    fwrite(&*header, 1, sizeof(*header), fh);

    // Make the partition file SIZE bytes long + header
    fseek(fh, (size - 1) + sizeof(*header) + 10, SEEK_SET);
    fputc('\0', fh);

    fclose(fh);

    return Partition(header, path);
}


