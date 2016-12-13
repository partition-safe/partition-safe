//
// Created by Youri Lefers on 13-12-16.
//

#ifndef PARTITIONSAFE_TREEENTRY_H
#define PARTITIONSAFE_TREEENTRY_H


#include "Entry.h"

class TreeEntry : public Entry {
public:
    /**
     * The tree entry constructor.
     *
     * @param filinfo The raw file info
     * @param directory The directory of the entry
     */
    TreeEntry(const FILINFO &filinfo, const std::string &directory);

};


#endif //PARTITIONSAFE_TREEENTRY_H
