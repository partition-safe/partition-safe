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
    Partition *partition;

public:
    /**
     * Get the partition instance.
     *
     * @return The partition instance
     */
    Partition *getPartition();

    /**
     * Partition header information struct.
     *
     * NEVER, NEVER, NEVER CHANGE THE ORDER OF THIS STRUCT.
     * WHO CHANGES THE ORDER, WILL BE ASSASSINATED.
     */
    struct Header {
        /**
         * Identifier of the vault
         */
        unsigned char identifier[14];

        /**
         * Custom partition label
         */
        char label[40];

        /**
         * The personal UUID of the vault.
         */
        char UUID[36];

        /**
         * The encrypted identifier.
         */
        unsigned char identifier_encrypted[24];

        /**
         * The current version of the partition.
         */
        unsigned int version;

        /**
         * Partition size in bytes
         */
        unsigned long size;
    };

    /**
     * The vault constructor.
     *
     * @param partition The partition to use
     */
    Vault(Partition *partition);

    /**
     * Vault destructor to close some stuff.
     */
    ~Vault();

    /**
     * Initialize a vault instance.
     *
     * @param vaultPath The path to the vault file
     *
     * @return The opened vault instance
     */
    static Vault *init(const char *vaultPath);

    /**
     * Create a new partition.
     *
     * @param label The label of the partition
     * @param size The size of the partition in bytes
     * @param path The path to place the new partition
     * @param encryptedIdentifier The encrypted identifier
     */
    static void create(const char label[40], const unsigned size, const char *path,
                       const unsigned char *encryptedIdentifier);

    /**
     * Open the vault and all internal properties.
     *
     * @return The opened vault instance
     */
    Vault *open();

    /**
     * The vault header
     */
    Header *header;

};


#endif //PARTITIONSAFE_VAULT_H
