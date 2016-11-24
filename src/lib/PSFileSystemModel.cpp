//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"


PSFileSystemModel::PSFileSystemModel(QObject *parent, PartitionSafe* psInstance):
    QAbstractItemModel(parent), psInstance(psInstance) {}

PSFileSystemModel::~PSFileSystemModel()
{

}

QModelIndex PSFileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
    return parent;
}

QModelIndex PSFileSystemModel::parent(const QModelIndex &child) const
{
    return child;
}

int PSFileSystemModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int PSFileSystemModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant PSFileSystemModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
