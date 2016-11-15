//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITIONSAFE_H
#define PARTITIONSAFE_PARTITIONSAFE_H


#include "Partition.h"

class PartitionSafe {

public:

    /**
     * The first few bytes of a partition, used as identifaction
     */
    const static char* IDENTIFIER;


    Partition createPartition(std::string label, unsigned int size, const char* path);

    void createKeyStore();

};


#endif //PARTITIONSAFE_PARTITIONSAFE_H
