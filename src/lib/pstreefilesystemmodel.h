#ifndef PSTREEFILESYSTEMMODEL_H
#define PSTREEFILESYSTEMMODEL_H

#include <PartitionSafe.h>
#include <QAbstractItemModel>


class PSTreeFileSystemModel : public QAbstractItemModel
{
    /**
     * @brief psInstance The internal partition safe instance
     */
    PartitionSafe* psInstance;

    /**
     * @brief currentDirectory The current directory path
     */
    TreeEntry *currentDirectory;

    /**
     * @brief setupData Setup the directory data
     */
    void setupData(TreeEntry **node, const char *path);

public:
    /**
     * @brief PSFileSystemModel File system model constructor.
     * @param parent
     */
    PSTreeFileSystemModel(QObject *parent = Q_NULLPTR, PartitionSafe* psInstance = Q_NULLPTR);

    /**
      * Destructor
      */
    ~PSTreeFileSystemModel();

    /**
     * @brief init Initialize the model.
     */
    void init();

    /**
     * @brief getCurrentDirectory Get the current directory
     * @return
     */
    QString getCurrentDirectory();

    /**
     * @brief setCurrentDirectory
     * @param path
     */
    void setCurrentDirectory(QString path);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
};

#endif // PSTREEFILESYSTEMMODEL_H
