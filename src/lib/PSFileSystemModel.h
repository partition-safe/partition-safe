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


    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    int columnCount(const QModelIndex &parent) const;

};


#endif //PSFILESYSTEMMODEL_H
