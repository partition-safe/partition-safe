//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITIONSAFE_H
#define PARTITIONSAFE_PARTITIONSAFE_H


#include "Vault.h"

class PartitionSafe {
    /**
     * The current vault instance.
     */
    Vault* vault;

public:
    /**
     * Create a new vault with key store.
     *
     * @param vaultPath The path to place the new vault
     * @param keyStorePath The path to place the new key store
     * @param label The label of the partition
     * @param size The size of the partition in bytes
     */
    void create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size);

    /**
     * Initialize a vault instance.
     *
     * @param vaultPath The path to the vault file
     * @param keyStorePath The path to the key store file
     *
     * @return
     */
    PartitionSafe* init(const char* vaultPath, const char* keyStorePath);

    /**
     * Open the partition safe instance.
     *
     * @return
     */
    PartitionSafe* open();

    /**
     * Write to a file on the partition.
     *
     * @param fileName
     * @param buff
     * @param size
     *
     * @return
     */
    PartitionSafe* writeFile(const std::string fileName, const void* buff, const UINT size);

    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    PartitionSafe* fileInfo(const std::string fileName, FILINFO *fileInfo);

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName
     * @param buff
     *
     * @return
     */
    PartitionSafe* readFile(const std::string fileName, void *buff, const UINT size);

};


#endif //PARTITIONSAFE_PARTITIONSAFE_H
