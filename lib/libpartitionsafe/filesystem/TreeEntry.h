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
     */
    TreeEntry();

    /**
     * The tree entry constructor.
     *
     * @param data The data entry
     * @param parent The parent tree entry
     * @param children The list of tree entries children
     */
    TreeEntry(const Entry *data, const TreeEntry *parent, std::vector<TreeEntry*> children = *(new std::vector<TreeEntry*>()));

    /**
     * Destructor
     */
    ~TreeEntry();

    /**
     * Add a new child to the children.
     *
     * @param child The child tree entry to add
     */
    void addChild(TreeEntry *child);

    /**
     * Get the list of child nodes.
     *
     * @return The vector of tree entry children
     */
    const std::vector<TreeEntry*> getChildren();

    /**
     * Get the parent item.
     *
     * @return The parent tree entry
     */
    const TreeEntry *getParent();

    /**
     * Retrieve the data of this node.
     *
     * @return The entry data
     */
    const Entry *getData();

    /**
     * Retrieve the row number.
     *
     * @return The row number
     */
    int row() const;

};


#endif //PARTITIONSAFE_TREEENTRY_H
