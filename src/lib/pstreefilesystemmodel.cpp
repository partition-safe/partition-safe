#include "lib/pstreefilesystemmodel.h"

PSTreeFileSystemModel::PSTreeFileSystemModel(QObject *parent, PartitionSafe *psInstance):
    QAbstractItemModel(parent), psInstance(psInstance)
{}

PSTreeFileSystemModel::~PSTreeFileSystemModel()
{
    if(currentDirectory) delete currentDirectory;
}

void PSTreeFileSystemModel::setupData(TreeEntry **node, const char *path)
{
    // Load data for this path
    psInstance->getVault()->getPartition()->listDirectory(path, node, directoriesOnly);

    // Iterate over all children of this node
    for(unsigned i = 0; i < (*node)->getChildren().size(); ++i) {
        // Retrieve the child
        TreeEntry *child = (*node)->getChildren().at(i);

        // Is the child a directory?
        if(child->getData()->isDirectory()) {
            setupData(&child, child->getData()->getFullPath().c_str());
        }
    }
}

void PSTreeFileSystemModel::init()
{
    currentDirectory = new TreeEntry();
    setupData(&currentDirectory, "/");
}

QString PSTreeFileSystemModel::getCurrentDirectory()
{
    const char *path = currentDirectory->getData()->name.c_str();
    return QString(path);
}

QModelIndex PSTreeFileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
    // Index existing?
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // The current parent item
    TreeEntry *parentItem;

    // Parent valid?
    if (!parent.isValid()) {
        parentItem = currentDirectory;
    } else {
        parentItem = static_cast<TreeEntry*>(parent.internalPointer());
    }

    // Retrieve the child item
    TreeEntry *childItem = parentItem->getChildren().at(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex PSTreeFileSystemModel::parent(const QModelIndex &index) const
{
    // Index valid?
    if (!index.isValid())
        return QModelIndex();

    TreeEntry *childItem = static_cast<TreeEntry*>(index.internalPointer());
    const TreeEntry *parentItem = childItem->getParent();

    if (parentItem == currentDirectory)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, (void *)parentItem);
}

int PSTreeFileSystemModel::rowCount(const QModelIndex &parent) const
{
    TreeEntry *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = currentDirectory;
    else
        parentItem = static_cast<TreeEntry*>(parent.internalPointer());

    return parentItem->getChildren().size();
}

int PSTreeFileSystemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant PSTreeFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeEntry *item = static_cast<TreeEntry*>(index.internalPointer());

    return QVariant(item->getData()->name.c_str());
}

void PSTreeFileSystemModel::setCurrentDirectory(QString path)
{

}

void PSTreeFileSystemModel::setDirectoriesOnly(const bool state)
{
    directoriesOnly = state;
}
