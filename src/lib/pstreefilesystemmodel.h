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
     * @brief directoriesOnly Only show directories
     */
    bool directoriesOnly = false;

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

    /**
     * @brief getCurrentDirectory Get the current directory
     * @return
     */
    QString getCurrentDirectory();

    /**
     * @brief headerData
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief setCurrentDirectory
     * @param path
     */
    void setCurrentDirectory(QString path);

    /**
     * @brief setDirectoriesOnly Set to show only directories
     */
    void setDirectoriesOnly(const bool state);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
};

#endif // PSTREEFILESYSTEMMODEL_H
