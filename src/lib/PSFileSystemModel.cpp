//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"


PSFileSystemModel::PSFileSystemModel(QObject *parent, PartitionSafe* psInstance):
    QAbstractListModel(parent), psInstance(psInstance)
{
    setCurrentDirectory(QString("/"));
}

PSFileSystemModel::~PSFileSystemModel()
{

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
    switch(role)
    {
    case Qt::DisplayRole:
        Entry* entry = currentDirectoryListing->at(index.row());
        return QVariant(QString(entry->name.c_str()));
    }
    return QVariant();
}

void PSFileSystemModel::setCurrentDirectory(QString path)
{
    currentDirectory = path;
    currentDirectoryListing = psInstance->getVault()->getPartition()->listDirectory(path.toStdString());
}
