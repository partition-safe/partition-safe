//
// Created by Youri Lefers on 24-11-16.
//

#ifndef PSFILESYSTEMMODEL_H
#define PSFILESYSTEMMODEL_H

#include <QAbstractItemModel>

class PSFileSystemModel : public QAbstractItemModel {
public:
    /**
     * @brief PSFileSystemModel File system model constructor.
     * @param parent
     */
    PSFileSystemModel(QObject *parent = Q_NULLPTR);

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

};


#endif //PSFILESYSTEMMODEL_H
