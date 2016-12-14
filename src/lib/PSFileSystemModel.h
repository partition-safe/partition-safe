//
// Created by Youri Lefers on 24-11-16.
//

#ifndef PSFILESYSTEMMODEL_H
#define PSFILESYSTEMMODEL_H

#include <PartitionSafe.h>
#include <QAbstractItemModel>

class PSFileSystemModel : public QAbstractListModel {
    /**
     * @brief psInstance The internal partition safe instance
     */
    PartitionSafe* psInstance;

    /**
     * @brief currentDirectory The current directory path
     */
    QString currentDirectory;

    /**
     * @brief currentDirectoryListing The contents of the current directory
     */
    std::vector<Entry*>* currentDirectoryListing;

public:
    /**
     * @brief PSFileSystemModel File system model constructor.
     * @param parent
     */
    PSFileSystemModel(QObject *parent = Q_NULLPTR, PartitionSafe* psInstance = Q_NULLPTR);

    /**
      * Destructor
      */
    ~PSFileSystemModel();

    /**
     * @brief setCurrentDirectory
     * @param path
     */
    void setCurrentDirectory(QString path);

    /**
     * @brief getCurrentDirectory Get the current directory
     * @return
     */
    QString getCurrentDirectory();

    /**
     * @brief getFile Get a file by it's index
     * @param index
     * @return
     */
    Entry* getFile(const QModelIndex &index) const;

    /**
     * @brief rowCount Get the number of rows in the current directory
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex &parent) const;

    /**
     * @brief data Data handling for a given index
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief columnCount Get the number of visible columns
     * @param parent
     * @return
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief headerData
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief init Initialize the model.
     */
    void init();

    void importFile(char* source, char* destination);
    void deleteFileDirectory(QModelIndexList &selectedRowsList);
    void deleteFileDirectory(QString path);
    void createDirectory( QString directoryName);
    void navigation_buttons(QStack<QString> &stackToCheck, QStack<QString> &stackToAdd);
    void enterDirectory(QString path, QStack<QString> &folderHistory, QStack<QString> &folderForwardHistory);
};


#endif //PSFILESYSTEMMODEL_H
