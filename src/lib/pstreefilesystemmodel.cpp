#include "lib/pstreefilesystemmodel.h"

#include <QIcon>

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

QVariant PSTreeFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        switch(section)
        {
        case 0:
            return QVariant("Directory");
        }
    }
    return QVariant();
}

void PSTreeFileSystemModel::init()
{
    currentDirectory = new TreeEntry();
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    setupData(&currentDirectory, "/");
    endInsertRows();
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

    if(parentItem != NULL){
        return createIndex(parentItem->row(), 0, (void *)parentItem);
    }
    return QModelIndex();
}

int PSTreeFileSystemModel::rowCount(const QModelIndex &parent) const
{
    TreeEntry *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid()) {
        if(currentDirectory) {
            parentItem = currentDirectory;
        } else return 0;
    } else
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

    TreeEntry *item = static_cast<TreeEntry*>(index.internalPointer());

    switch(role)
    {
    case Qt::DecorationRole:
        if(item->getData()->isDirectory()){
            return *new QIcon(QString::fromStdString(":/resource/ic_folder_black_48dp.png"));
        }else{
            return *new QIcon(QString::fromStdString(":/resource/ic_insert_drive_file_black_48dp.png"));
        }
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0:
            return QVariant(item->getData()->name.c_str());
        }

    }
    return QVariant();
}

void PSTreeFileSystemModel::setCurrentDirectory(QString path)
{

}

void PSTreeFileSystemModel::setDirectoriesOnly(const bool state)
{
    directoriesOnly = state;
}
