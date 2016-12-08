//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "Vault.h"

Vault::Vault(Partition *partition):
    partition(partition) {}

void Vault::create(const char *label, const unsigned size, const char *path, const unsigned char *encryptedIdentifier) {
    // Open the file path
    FILE* fd = fopen(path, "w");

    // Check the file descriptor
    if(fd == nullptr) {
        throw "Could not open the file";
    }

    // Create the vault header
    struct Header* header = new Header;
    memcpy((void *)header->identifier, Partition::IDENTIFIER, sizeof(header->identifier));
    strncpy(header->label, label, sizeof(header->label));
    strncpy(header->UUID, "23456", sizeof(header->UUID)); // @TODO Use real UUID
    header->size = size;
    header->version = Partition::VERSION;
    memcpy((void *)header->identifier_encrypted, encryptedIdentifier, sizeof(header->identifier_encrypted));

    // Write our partition header to the file
    fwrite(&*header, 1, sizeof(*header), fd);

    // Make the partition file SIZE bytes long + header
    fseek(fd, (size - 1) + sizeof(*header) + 10, SEEK_SET);
    fputc('\0', fd);

    // Open the partition
    Partition* partition = new Partition(path, fd);

    // Let it create the file system
    partition->create();

    // Close the file descriptor
    fclose(fd);
}

Vault *Vault::init(const char* vaultPath) {
    // Open the vault file
    FILE* vaultFileDescriptor = fopen(vaultPath, "r+");

    // Check both descriptors
    if(vaultFileDescriptor == nullptr) {
        throw "Could not open vault file";
    }

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, vaultFileDescriptor);

    // Check the identifier of the drive
    if(memcmp(header->identifier, Partition::IDENTIFIER, sizeof(header->identifier)) != 0) {
        throw "Header identifier does not match partition identifier";
    }

    // Open the partition and set information
    Partition* partition = new Partition(vaultPath, vaultFileDescriptor);

    // Create the vault
    Vault* vault = new Vault(partition);
    vault->header = header;

    // Return the vault
    return vault;
}

Vault *Vault::open() {
    // Open the partition
    partition->open();

    // Return myself
    return this;
}

Partition *Vault::getPartition() {
    return partition;
}
