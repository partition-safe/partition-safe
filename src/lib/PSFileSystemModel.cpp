//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"


PSFileSystemModel::PSFileSystemModel(QObject *parent, PartitionSafe* psInstance):
    QAbstractItemModel(parent), psInstance(psInstance)
{
    setCurrentDirectory(QString("/"));
}

PSFileSystemModel::~PSFileSystemModel()
{

}

QModelIndex PSFileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex PSFileSystemModel::parent(const QModelIndex &child) const
{
    return child;
}

int PSFileSystemModel::rowCount(const QModelIndex &parent) const
{
    return currentDirectoryListing->size();
}

int PSFileSystemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant PSFileSystemModel::data(const QModelIndex &index, int role) const
{
    return QVariant("test");
}

void PSFileSystemModel::setCurrentDirectory(QString path)
{
    currentDirectory = path;
    currentDirectoryListing = psInstance->getVault()->getPartition()->listDirectory(path.toStdString());
}
