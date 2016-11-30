//
// Created by marc on 15-11-16.
//

#include "PartitionSafe.h"
#include "Common.h"

void PartitionSafe::create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size) {
    // Try to create the vault
    Vault::create(label, size, vaultPath);

    // Try to create the key store
    KeyStore::create(keyStorePath);
}

PartitionSafe *PartitionSafe::init(const char* vaultPath, const char* keyStorePath) {
    // Get the vault instance
    vault = Vault::init(vaultPath);

    // Get the key store instance
    keyStore = KeyStore::init(keyStorePath);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::open() {
    // Open the vault
    vault->open();

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::writeFile(const std::string fileName, const void *buff, const UINT size) {
    // Write the file
    vault->writeFile(Common::stdStringToTChar(fileName), buff, size);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::fileInfo(const std::string fileName, FILINFO *fileInfo) {
    // Get file info
    vault->fileInfo(Common::stdStringToTChar(fileName), fileInfo);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::readFile(const std::string fileName, void *buff, const UINT size) {
    // Read the file
    vault->readFile(Common::stdStringToTChar(fileName), buff, size);

    // Return myself
    return this;
}

Vault *PartitionSafe::getVault() {
    return vault;
}

KeyStore *PartitionSafe::getKeyStore() {
    return keyStore;
}

int PartitionSafe::importFile(const char *source, const char *destination) {
    return vault->importFile(source, destination);
}

int PartitionSafe::exportFile(const char *source, const char *destination) {
    return vault->exportFile(source, destination);
}
