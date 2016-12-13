//
// Created by Youri Lefers on 24-11-16.
//

#ifndef PARTITIONSAFE_ENTRY_H
#define PARTITIONSAFE_ENTRY_H


#include <string>
#include "../../libfatfs/src/ff.h"

class Entry {
public:
    /**
     * The entry size
     */
    const FSIZE_t size;

    /**
     * The modified date
     */
    const WORD date;

    /**
     * The modified time
     */
    const WORD time;

    /**
     * Entry attribute
     */
    const BYTE attribute;

    /**
     * Entry alternate name
     */
    const std::string altName;

    /**
     * Entry name
     */
    const std::string name;

    /**
     * Directory name
     */
    const std::string directory;

    /**
     * The entry constructor.
     *
     * @param filinfo The raw file info
     * @param directory The directory of the entry
     */
    Entry(const FILINFO filinfo, const std::string directory);

    /**
     * Get the full path of this entry.
     * @return
     */
    const std::string getFullPath();

    /**
     * Is the entry a directory.
     * @return
     */
    bool isDirectory();

};


#endif //PARTITIONSAFE_ENTRY_H
