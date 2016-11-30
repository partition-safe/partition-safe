//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"

#include <mainwindow/mainwindow.h>

PSFileSystemModel::PSFileSystemModel(QObject *parent, PartitionSafe* psInstance):
    QAbstractListModel(parent), psInstance(psInstance)
{}

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

QVariant PSFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        switch(section)
        {
        case 0:
            return QVariant("Filename");
        }
    }
    return QVariant();
}

void PSFileSystemModel::init()
{
    setCurrentDirectory(QString("/"));
}

QVariant PSFileSystemModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        Entry* entry = currentDirectoryListing->at(index.row());
        switch (index.column()) {
        case 0: return QString(entry->name.c_str());
            // case 1: return entry->size;
        }

    }
    return QVariant();
}

void PSFileSystemModel::setCurrentDirectory(QString path)
{
    currentDirectory = path;
    currentDirectoryListing = psInstance->getVault()->getPartition()->listDirectory(path.toStdString());
    emit dataChanged(index(0), index(rowCount(QModelIndex())));
}

void PSFileSystemModel::importFile(char* source, char* destination){
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    psInstance->importFile(source, destination);
    endInsertRows();

    setCurrentDirectory(getCurrentDirectory());
}

void PSFileSystemModel::deleteFileDirectory(const char *source){
    beginRemoveRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    psInstance->deleteFileDirectory(source);
    endRemoveRows();

    setCurrentDirectory(getCurrentDirectory());
}

QString PSFileSystemModel::getCurrentDirectory()
{
    return currentDirectory;
}

Entry *PSFileSystemModel::getFile(const QModelIndex &index) const
{
    return currentDirectoryListing->at(index.row());
}
