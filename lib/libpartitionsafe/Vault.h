//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_VAULT_H
#define PARTITIONSAFE_VAULT_H


#include "Partition.h"

class Vault {
    /**
     * Our file storage
     */
    Partition* partition;

public:
    /**
     * Get the partition instance.
     * @return
     */
    Partition* getPartition();

    /**
     * Partitio header information struct.
     *
     * NEVER, NEVER, NEVER CHANGE THE ORDER OF THIS STRUCT.
     * WHO CHANGES THE ORDER, WILL BE ASSASSINATED.
     */
    struct Header {
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

    /**
     * The vault constructor.
     *
     * @param partition
     * @param keyStore
     */
    Vault(Partition* partition);

    /**
     * Open a vault instance.
     *
     * @param vaultPath The path to the vault file
     *
     * @return The opened vault instance
     */
    static Vault* init(const char* vaultPath);

    /**
     * Create a new partition.
     *
     * @param label The label of the partition
     * @param size The size of the partition in bytes
     * @param path The path to place the new partition
     *
     * @return
     */
    static void create(const char label[40], const unsigned size, const char* path);

    /**
     * Open the file and all internal properties.
     *
     * @param fs
     *
     * @return
     */
    Vault* open();

    /**
     * Write to a file on the partition.
     *
     * @param fileName
     * @param buff
     * @param size
     *
     * @return
     */
    Vault* writeFile(const TCHAR* fileName, const void* buff, const UINT size);

    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    Vault* fileInfo(const TCHAR *fileName, FILINFO *fileInfo);

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName
     * @param buff
     *
     * @return
     */
    Vault* readFile(const TCHAR *fileName, void *buff, const UINT size);

    int importFile(const char* source, const char* destination);
    Partition* deleteFileDirectory(const char* source);

    int exportFile(const char* source, const char* destination);

private:
    /**
     * The vault header
     */
    Header* header;

};


#endif //PARTITIONSAFE_VAULT_H
