//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITION_H
#define PARTITIONSAFE_PARTITION_H

#include <iostream>

class Partition {

public:
    struct Header{
        char identifier[5];
        char label[40];
        unsigned int size;
    };

private:


    /**
   * The partition header
   */
    Header* header;

    /**
     * File path to this partition
     */
    const char* path;

    FILE * fh;

public:
    static const char* IDENTIFIER;

    Partition(Header* header, const char* path);

    /**
     * Open the partition
     */
    static Partition open(const char* path);

    static Partition create(char label[40], unsigned int size, const char* path);

};


#endif //PARTITIONSAFE_PARTITION_H
