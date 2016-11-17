//
// Created by marc on 15-11-16.
//

#include <cstring>
#include <sys/file.h>
#include <unistd.h>
#include "Partition.h"

const char* Partition::IDENTIFIER = "PS01";

Partition::Partition(Header* header, const char* path) {
    this->header = header;
    this->path = path;
}

Partition Partition::open(const char* path) {
    FILE* fh = fopen(path, "r+");
    if(flock(fileno(fh), LOCK_EX) == 0){
        std::cout << "All good"<< std::endl;
    }else{
        std::cout << "Could not acquire lock" << std::endl;
    }

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, fh);

    if(strcmp(header->identifier, Partition::IDENTIFIER) == 0){
        std::cout << "Partition: Good";
    }else{
        std::cout << "Partition: Bad, " << header->identifier;
    }

    std::cout<< std::endl;
    std::cout << "Label: " << header->label << std::endl;
    std::cout << "Size: " << header->size << std::endl;

    Partition partition = Partition(header, path);
    partition.fh = fh;

    return partition;
}

Partition Partition::create(char label[40], unsigned int size, const char* path) {
    FILE* fh = fopen(path, "w");

    char identifier[5];
    strncpy(identifier, Partition::IDENTIFIER, 5);

    // The partition header
    struct Header* header = new Header;
    strcpy(header->identifier, identifier);
    strcpy(header->label, label);
    header->size = size;

    // Write our partition header
    fwrite(&*header, 1, sizeof(*header), fh);

    // Make the partition file SIZE bytes long + header
    fseek(fh, (size - 1) + sizeof(*header) + 10, SEEK_SET);
    fputc('\0', fh);

    fclose(fh);

    return Partition(header, path);
}