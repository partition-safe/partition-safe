//
// Created by marc on 15-11-16.
//

#include <iostream>
#include "PartitionSafe.h"
#include "Common.h"

int main() {
    std::cout << "PartitionSafe :: Test script" << std::endl << std::endl;

    try {

        //
        // Create vault
        //

        // Vault metadata
        const char *vaultPath = "/tmp/marc.vault";
        const char *keyStorePath = "/tmp/marc.keystore";
        char label[40] = "Marc";

        // Create the partition safe instance
        PartitionSafe *ps = new PartitionSafe();

        // Create the vault
        std::cout << "-- Partition create" << std::endl;
        ps->create(vaultPath, keyStorePath, label, 1024);

        //
        // Open vault
        //

        // Init the vault
        std::cout << "-- Partition open" << std::endl;
        ps->init(vaultPath, keyStorePath)->open();

        //
        // Write file
        //

        // File content
        const std::string filename = "/sample.txt";
        const std::string filename2 = "/sample2222222222.txt";
        const char line[] = "Hello world\nHai";

        // Write content
        std::cout << "-- File write" << std::endl;
        ps->writeFile(filename, line, sizeof(line));
        ps->writeFile(filename2, line, sizeof(line));

        //
        // Open file
        //

        // File metadata
        FILINFO fileInfo;

        // Get file info
        ps->fileInfo(filename, &fileInfo);

        // The file buffer
        char readLines[fileInfo.fsize];

        // Read content
        std::cout << "-- Read from file: " << std::endl;
        ps->readFile(filename, readLines, sizeof(readLines));
        std::cout << readLines << std::endl;

        //
        // Create directory and populate
        //

        // The directory name
        const std::string directoryName = "Test directory";

        // Create directory
        ps->getVault()->getPartition()->createDirectory(directoryName);
        ps->writeFile(directoryName + "\\" + filename, line, sizeof(line));
        ps->writeFile(directoryName + "\\" + filename2, line, sizeof(line));

        //
        // Read directory structure
        //

        // Read directories/files for root
        std::cout << "-- List directories" << std::endl;
        std::vector<Entry*>* entries = ps->getVault()->getPartition()->listDirectory(Common::stdStringToTChar("/"));

        // Print entries
        for(Entry* const& value : *entries) {
            std::cout << value->getFullPath() << std::endl;
        }
    } catch(const char* exception) {
        // Hey, exception
        std::cout << "Thrown exception: " << exception << std::endl;
    }

    return 0;
}
