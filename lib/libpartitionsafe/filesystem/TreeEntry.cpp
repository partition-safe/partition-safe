//
// Created by Youri Lefers on 13-12-16.
//

#include "TreeEntry.h"

TreeEntry::TreeEntry():
        data(nullptr), parent(nullptr), children(*new std::vector<TreeEntry*>()) {}

TreeEntry::TreeEntry(const Entry *data, const TreeEntry *parent, std::vector<TreeEntry*> children):
        data(data), parent(parent), children(children) {}

void TreeEntry::addChild(TreeEntry *child) {
    children.push_back(child);
}

const std::vector<TreeEntry*> TreeEntry::getChildren() {
    return children;
}

const TreeEntry *TreeEntry::getParent() {
    return parent;
}

const Entry *TreeEntry::getData() {
    return data;
}

int TreeEntry::row() const
{
    if (parent) {
        return std::find(parent->children.begin(), parent->children.end(), this) - parent->children.begin();
    }

    return 0;
}
