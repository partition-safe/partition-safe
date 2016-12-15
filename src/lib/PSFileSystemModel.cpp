//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"

#include <mainwindow/mainwindow.h>
#include <Common.h>
#include <QDebug>

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
    Entry* entry = currentDirectoryListing->at(index.row());

    switch(role)
    {
    case Qt::DecorationRole:
        if(entry->isDirectory()){
            return QIcon::fromTheme("folder");
        }else{
             return QIcon::fromTheme("text-x-generic");
        }
    case Qt::DisplayRole:
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
    currentDirectoryListing = (std::vector<Entry*>*) psInstance->getVault()->getPartition()->listDirectory(path.toStdString());
    emit dataChanged(index(0), index(rowCount(QModelIndex())));
}

void PSFileSystemModel::importFile(const char* source, const char* destination)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    psInstance->getVault()->getPartition()->importFile(source, destination);
    endInsertRows();

    setCurrentDirectory(getCurrentDirectory());
}

void PSFileSystemModel::deleteFileDirectory(QModelIndexList &selectedRowsList)
{
    foreach (QModelIndex index, selectedRowsList)
    {
        deleteFileDirectory(this->getFile(index)->getFullPath().data());
    }
    setCurrentDirectory(getCurrentDirectory());
}

void PSFileSystemModel::deleteFileDirectory(QString path)
{
    FRESULT exists;
    FILINFO fno;
    std::vector<Entry*>* subDirectoryListing;

    Entry* entry;

    beginRemoveRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    exists = f_stat(Common::stdStringToTChar(path.toStdString()), &fno);
    if(exists == FR_OK){
        switch(fno.fattrib){
        case AM_DIR:
            qDebug()<< "directory";
            subDirectoryListing = (std::vector<Entry*>*) psInstance->getVault()->getPartition()->listDirectory(path.toStdString());
            while(subDirectoryListing->size()>=1){
                entry = subDirectoryListing->back();
                subDirectoryListing->pop_back();

                if(entry->isDirectory()) deleteFileDirectory(entry->getFullPath().c_str());
                else psInstance->getVault()->getPartition()->deleteFileDirectory(entry->getFullPath().c_str());
            }
            if(subDirectoryListing->size()<=0) psInstance->getVault()->getPartition()->deleteFileDirectory(path.toStdString().c_str());
            break;
        default:
            psInstance->getVault()->getPartition()->deleteFileDirectory(path.toStdString().c_str());
            break;
        }
    }
    endRemoveRows();
}

QString PSFileSystemModel::getCurrentDirectory()
{
    return currentDirectory;
}

Entry *PSFileSystemModel::getFile(const QModelIndex &index) const
{
    return currentDirectoryListing->at(index.row());
}

void PSFileSystemModel::navigation_buttons(QStack<QString> &stackToCheck,QStack<QString> &stackToAdd )
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));

    // More than only the home item
    if(stackToCheck.size() > 0)
    {
        // Get the first item of the stack
        QString dir = stackToCheck.pop();

        // Add to folderForwardHistory
        stackToAdd.append(getCurrentDirectory());

        // Set directory
        setCurrentDirectory(dir);
    }
    endInsertRows();
}

void PSFileSystemModel::enterDirectory(QString dirToEnter, QStack<QString> &folderHistory, QStack<QString> &folderForwardHistory)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));

    // Add history to stack
    folderHistory.append(this->getCurrentDirectory());

    // Set directory
    this->setCurrentDirectory(dirToEnter);

    // clear forward if it doesn't contain dir
    // else pop from ForwardHistory
    if (!folderForwardHistory.contains(dirToEnter)) folderForwardHistory.clear();
    else folderForwardHistory.pop();

    endInsertRows();

}
