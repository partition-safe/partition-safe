//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"


PSFileSystemModel::PSFileSystemModel(QObject *parent = Q_NULLPTR):
    QAbstractItemModel(parent) {}

QModelIndex QAbstractItemModel::index(int row, int column, const QModelIndex &parent) const
{
    return NULL;
}

QModelIndex QAbstractItemModel::parent(const QModelIndex &child) const
{
    return NULL;
}

int QAbstractItemModel::rowCount(const QModelIndex &parent) const
{
    return NULL;
}

int QAbstractItemModel::columnCount(const QModelIndex &parent) const
{
    return NULL;
}

QVariant QAbstractItemModel::data(const QModelIndex &index, int role) const
{
    return NULL;
}
