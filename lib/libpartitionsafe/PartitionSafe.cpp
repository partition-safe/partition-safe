//
// Created by marc on 15-11-16.
//

#include "PartitionSafe.h"


Partition PartitionSafe::createPartition(std::string label, unsigned int size, const char* path) {
    FILE* fh = fopen(path, "w");

    // Write our identification bytes
    fwrite(PartitionSafe::IDENTIFIER , 1 , sizeof(PartitionSafe::IDENTIFIER) , fh );

    // Make the partition file SIZE bytes long
    fseek(fh, size-1, SEEK_SET);
    fputc('\0', fh);

    fclose(fh);

    return Partition(label, size, path);
}

const char* PartitionSafe::IDENTIFIER = "PS01";