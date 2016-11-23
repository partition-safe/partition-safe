//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>

class KeyStore {
    /**
     * File path to this partition
     */
    const char* path;

public:
    /**
     * The file descriptor of this partition.
     *
     * This descriptor contains the real partition file.
     */
    FILE * fd;

    /**
     * Constructor of a partition.
     *
     * @param path
     * @param fh
     * @return
     */
    KeyStore(const char* path, FILE* fh = nullptr);

};


#endif //PARTITIONSAFE_KEYSTORE_H
