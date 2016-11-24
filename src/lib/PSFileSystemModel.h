//
// Created by Youri Lefers on 24-11-16.
//

#ifndef PSFILESYSTEMMODEL_H
#define PSFILESYSTEMMODEL_H

#include <PartitionSafe.h>
#include <QAbstractItemModel>

class PSFileSystemModel : public QAbstractItemModel {
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
     * @brief index
     * @param row
     * @param column
     * @param parent
     * @return
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief parent
     * @param child
     * @return
     */
    QModelIndex parent(const QModelIndex &child) const;

    /**
     * @brief rowCount
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief columnCount
     * @param parent
     * @return
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief data
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief setCurrentDirectory
     * @param path
     */
    void setCurrentDirectory(QString path);

};


#endif //PSFILESYSTEMMODEL_H
