//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "Partition.h"
#include "PartitionSafe.h"

Partition::Partition(std::string label, unsigned int size, const char* path) {
    this->label = label;
    this->size = size;
    this->path = path;
}

void Partition::open() {
    fh = fopen(path, "r+");

    char identifier[4];

    fread(identifier, strlen(identifier) + 2, 2, fh);

    if(strcmp(identifier, PartitionSafe::IDENTIFIER) == 0){
        std::cout << "Partition: Good";
    }else{
        std::cout << "Partition: Bad, " << identifier;
    }
}