//
// Created by Youri Lefers on 24-11-16.
//

#include "PSFileSystemModel.h"

#include <mainwindow/mainwindow.h>
#include <Common.h>
#include <QDebug>
#include <QDir>

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
            return *new QIcon(QString::fromStdString(":/resource/ic_folder_black_48dp.png"));
        }else{
            return *new QIcon(QString::fromStdString(":/resource/ic_insert_drive_file_black_48dp.png"));
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

void PSFileSystemModel::importFolder(QModelIndexList &selectedRowsList, const char* destination)
{
    foreach (QModelIndex index, selectedRowsList)
    {
        importFolder(this->getFile(index)->getFullPath().data(), destination);
    }
    setCurrentDirectory(getCurrentDirectory());
}

void PSFileSystemModel::importFolder(QString source, QString destination)
{
    FRESULT exists;
    FILINFO fno;

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));

    QDir impDir;
    impDir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    if(impDir.cd(source)){
        QString newDestination = destination;
        newDestination = destination + "/" + impDir.dirName();
        //Check if folder already exists
        exists = f_stat(Common::stdStringToTChar(newDestination.toStdString()), &fno);
        if(exists != FR_OK){
            createDirectory(newDestination);
        }

        QFileInfoList list = impDir.entryInfoList();

        for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);

                if(fileInfo.isDir()){
                    importFolder(fileInfo.path() + "/" + fileInfo.fileName(), newDestination);
                }
                else{
                    //check if file already exists
                    QFile chFile(newDestination + "/" + fileInfo.fileName());
                    if(chFile.exists()){
                        chFile.remove();
                    }
                    psInstance->getVault()->getPartition()->importFile(fileInfo.absoluteFilePath().toLatin1().data(), (newDestination + "/" + fileInfo.fileName()).toLatin1().data());
                }
        }
    }
    else{
        qDebug() << "Map doesnt exists";
    }
    endInsertRows();
}

void PSFileSystemModel::deleteFileDirectory(QModelIndexList &selectedRowsList)
{
    foreach (QModelIndex index, selectedRowsList)
    {
        deleteFileDirectory(this->getFile(index)->getFullPath().data());
    }
    setCurrentDirectory(getCurrentDirectory());
}

void PSFileSystemModel::createDirectory( QString directoryName){
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    psInstance->getVault()->getPartition()->createDirectory(directoryName.toStdString());
    setCurrentDirectory(getCurrentDirectory());
    endInsertRows();
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
bool PSFileSystemModel::directoryExists(QString path){
    FRESULT exists;
    FILINFO fno;

    exists = f_stat(Common::stdStringToTChar(path.toStdString()), &fno);

    if(exists==FR_OK && fno.fattrib == AM_DIR) return true;
    else return false;
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
