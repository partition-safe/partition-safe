//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "Vault.h"

Vault::Vault(Partition *partition):
    partition(partition) {}

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

Vault *Vault::init(const char* vaultPath) {
    // Open the vault file
    FILE* vaultFileDescriptor = fopen(vaultPath, "r+");

    // Check both descriptors
    if(vaultFileDescriptor == NULL) {
        throw strerror(errno);
    }

    // Read the partition header
    Header* header = new Header;
    fread(&*header, sizeof(*header), 1, vaultFileDescriptor);

    // Check the identifier of the drive
    if(strcmp(header->identifier, Partition::IDENTIFIER) != 0) {
        throw "Header identifier does not match partition identifier";
    }

    // Open the partition and set information
    Partition* partition = new Partition(vaultPath, vaultFileDescriptor);

    // Create the vault
    Vault* vault = new Vault(partition);

    // Return the vault
    return vault;
}

Vault *Vault::open() {
    // Open the partition
    partition->open();

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

Vault *Vault::readFile(const TCHAR *fileName, void *buff, const UINT size) {
    // Write the file
    partition->readFile(fileName, buff, size);

    // Return myself
    return this;
}

Partition *Vault::getPartition() {
    return partition;
}

int Vault::importFile(const char *source, const char *destination) {
    return partition->importFile(source, destination);
}

Partition *Vault::deleteFileDirectory(const char *source) {
    return partition->deleteFileDirectory(source);
}

int Vault::exportFile(const char *source, const char *destination) {
    return partition->exportFile(source, destination);
}
