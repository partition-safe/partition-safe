//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITION_H
#define PARTITIONSAFE_PARTITION_H

#include <iostream>

class Partition {

    /**
     * Label name of the partition
     */
    std::string label;

    /**
     * Total size in bytes
     */
    unsigned int size;

    /**
     * File path to this partition
     */
    const char* path;

    FILE * fh;
public:
    Partition(std::string label, unsigned int size,  const char* path);

    /**
     * Open the partition
     */
    void open();

};


#endif //PARTITIONSAFE_PARTITION_H
