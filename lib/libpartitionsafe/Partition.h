//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITION_H
#define PARTITIONSAFE_PARTITION_H

#include <iostream>

class Partition {

public:
    /**
     * Partitio header information struct.
     *
     * NEVER, NEVER, NEVER CHANGE THE ORDER OF THIS STRUCT.
     * WHO CHANGES THE ORDER, WILL BE ASSASSINATED.
     */
    struct Header{
        /**
         * Identifier of the vault
         */
        char identifier[13];

        /**
         * The current version of the partition.
         */
        unsigned int version;

        /**
         * Custom partition label
         */
        char label[40];

        /**
         * Partition size in bytes
         */
        unsigned long size;

        /**
         * The personal UUID of the vault.
         */
        char UUID[36];
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

    /**
     * The file handler of this partition.
     *
     * This handler contains the real partition file.
     */
    FILE * fh;

public:
    /**
     * The identifier of the save.
     *
     * Used for testing decryption.
     */
    static const char* IDENTIFIER;

    /**
     * The save version for compatibility checks.
     */
    static const unsigned int VERSION;

    /**
     * Constructor of a partition.
     *
     * @param header
     * @param path
     * @param fh
     * @return
     */
    Partition(Header* header, const char* path, FILE* fh = nullptr);

    /**
     * Open the partition.
     *
     * @param path The path to the partition file
     * @return
     */
    static Partition open(const char* path);

    /**
     * Create a new partition.
     *
     * @param label The label of the partition
     * @param size The size of the partition in bytes
     * @param path The path to place the new partition
     * @return
     */
    static Partition create(char label[40], unsigned int size, const char* path);

};


#endif //PARTITIONSAFE_PARTITION_H
