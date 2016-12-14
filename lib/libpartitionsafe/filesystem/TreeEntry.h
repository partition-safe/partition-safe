//
// Created by Youri Lefers on 13-12-16.
//

#ifndef PARTITIONSAFE_TREEENTRY_H
#define PARTITIONSAFE_TREEENTRY_H


#include <vector>
#include "Entry.h"

class TreeEntry {
protected:
    /**
     * The parent tree entry.
     */
    const TreeEntry *parent;

    /**
     * The list of child entries.
     */
    std::vector<TreeEntry*> children;

    /**
     * The current entry of this tree entry.
     */
    const Entry *data;

public:
    /**
     * The tree entry constructor.
     *
     * @param filinfo The raw file info
     * @param directory The directory of the entry
     */
    TreeEntry();

    /**
     * The tree entry constructor.
     *
     * @param filinfo The raw file info
     * @param directory The directory of the entry
     */
    TreeEntry(const Entry *data, const TreeEntry *parent, std::vector<TreeEntry*> children = *(new std::vector<TreeEntry*>()));

    /**
     * Destructor
     */
    ~TreeEntry();

    /**
     * Add a new child to the children.
     *
     * @param child
     */
    void addChild(TreeEntry *child);

    /**
     * Get the list of child nodes.
     *
     * @return
     */
    const std::vector<TreeEntry*> getChildren();

    /**
     * Get the parent item.
     *
     * @return
     */
    const TreeEntry *getParent();

    /**
     * Retrieve the data of this node.
     *
     * @return
     */
    const Entry *getData();


    int row() const;

};


#endif //PARTITIONSAFE_TREEENTRY_H
