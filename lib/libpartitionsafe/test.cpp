//
// Created by marc on 15-11-16.
//

#include <iostream>
#include <fstream>
#include "PartitionSafe.h"
#include "Common.h"

int main() {
    std::cout << "PartitionSafe :: Test script" << std::endl << std::endl;

    try {

        //
        // Create vault
        //

        // Vault metadata
#ifndef __WIN32
        const char *vaultPath = "/tmp/marc.vault";
        const char *keyStorePath = "/tmp/marc.keystore";
#else
        char vaultPath[1024];
        char keyStorePath[1024];
        ExpandEnvironmentStrings("%Temp%\\marc.vault", vaultPath, 1024);
        ExpandEnvironmentStrings("%Temp%\\marc.keystore", keyStorePath, 1024);
#endif
        char label[40] = "Marc";

        // Delete old files
        std::remove(vaultPath);
        std::remove(keyStorePath);

        // Create the partition safe instance
        PartitionSafe *ps = new PartitionSafe();

        // Create the vault
        std::cout << "-- Partition create" << std::endl;
        ps->create(vaultPath, keyStorePath, label, 1024, "test", "test");

        //
        // Open vault
        //

        // Init the vault
        std::cout << "-- Partition open" << std::endl;
        ps->init(vaultPath, keyStorePath, "test", "test")->open();

        //
        // Write file
        //

        // File content
        const std::string filename1_1 = "/sample.txt";
        const std::string filename1_2 = "/sample2222222222.txt";
        const char line[] = "Hello world\nHai";

        // Write content
        std::cout << "-- File write" << std::endl;
        ps->getVault()->getPartition()->writeFile(filename1_1, line, sizeof(line));
        ps->getVault()->getPartition()->writeFile(filename1_2, line, sizeof(line));

        //
        // Open file
        //

        // File metadata
        FILINFO fileInfo;

        // Get file info
        ps->getVault()->getPartition()->fileInfo(filename1_1, &fileInfo);

        // The file buffer
        char readLines[fileInfo.fsize];

        // Read content
        std::cout << "-- Read from file: " << std::endl;
        ps->getVault()->getPartition()->readFile(filename1_1, readLines, sizeof(readLines));
        std::cout << readLines << std::endl;

        //
        // Create directory and populate
        //

        // The directory name
        const std::string filename2_1 = "/test.txt";
        const std::string filename2_2 = "/test 2.txt";
        const std::string directoryName = "Test directory";

        // Create directory
        ps->getVault()->getPartition()->createDirectory(directoryName);
        ps->getVault()->getPartition()->writeFile(directoryName + "\\" + filename2_1, line, sizeof(line));
        ps->getVault()->getPartition()->writeFile(directoryName + "\\" + filename2_2, line, sizeof(line));

        // Import a file
#ifndef __WIN32
        const char *importTest = "/tmp/test.txt";
#else
        char importTest[1024];
        ExpandEnvironmentStrings("%Temp%\\test.txt", importTest, 1024);
#endif
        std::ofstream outfile (importTest);
        outfile << "my text here!" << std::endl;
        outfile.close();
        ps->getVault()->getPartition()->importFile(importTest, "/marc.txt");

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

        // Cleanup
        std::cout << "-- Cleanup" << std::endl;
        delete ps;

        // Finish
        std::cout << "-- Finished test script! --" << std::endl;
    } catch(const char* exception) {
        // Hey, exception
        std::cout << "Thrown exception: " << exception << std::endl;
    }

    return 0;
}
