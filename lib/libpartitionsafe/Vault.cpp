//
// Created by marc on 15-11-16.
//

#include "Vault.h"

Vault::Vault(Partition *partition, KeyStore *keyStore):
    partition(partition), keyStore(keyStore) {}

void Vault::create(const char *label, const unsigned size, const char *path) {
    // Open the file path
    FILE* fd = fopen(path, "w");

    // Check the file descriptor
    if(fd == nullptr) {
        throw "Could not open the file";
    }

    // Create the vault header
    struct Header* header = new Header;
    strncpy(header->identifier, Partition::IDENTIFIER, sizeof(header->identifier));
    strncpy(header->label, label, sizeof(header->label));
    strncpy(header->UUID, "23456", sizeof(header->UUID)); // @TODO Use real UUID
    header->size = size;
    header->version = Partition::VERSION;

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

Vault *Vault::init(const char* vaultPath, const char* keyStorePath) {
    // Open the vault file
    FILE* vaultFileDescriptor = fopen(vaultPath, "r+");

    // Open the key store file
    FILE* keyStoreFileDescriptor = fopen(keyStorePath, "r+");

    // Check both descriptors
    if(vaultFileDescriptor == nullptr) {
        throw "Could not open vault file";
    } else if(keyStoreFileDescriptor == nullptr) {
        throw "Could not open key store file";
    }

    // Create the vault

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, vaultFileDescriptor);

    // Check the identifier of the drive
    if(strcmp(header->identifier, Partition::IDENTIFIER) != 0) {
        throw "Header identifier does not match partition identifier";
    }

    // Open the partition and set information
    Partition* partition = new Partition(vaultPath, vaultFileDescriptor);

    // Open the key store
    KeyStore* keyStore = new KeyStore(keyStorePath, keyStoreFileDescriptor);

    // Create the vault
    Vault* vault = new Vault(partition, keyStore);

    // Return the vault
    return vault;
}

Vault *Vault::open(FATFS* fs) {
    // Open the partition
    partition->open(fs);

    // Return myself
    return this;
}

Vault *Vault::writeFile(const TCHAR *fileName, const void *buff, const UINT size) {
    // Write the file
    partition->writeFile(fileName, buff, size);

    // Return myself
    return this;
}

Vault *Vault::fileInfo(const TCHAR *fileName, FILINFO *fileInfo) {
    // Write the file
    partition->fileInfo(fileName, fileInfo);

    // Return myself
    return this;
}

Vault *Vault::readFile(const TCHAR *fileName, void *buff) {
    // Write the file
    partition->readFile(fileName, buff);

    // Return myself
    return this;
}
