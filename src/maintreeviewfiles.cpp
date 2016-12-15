#include "maintreeviewfiles.h"
#include <QTreeView>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QFileInfo>
#include <lib/PSFileSystemModel.h>


MainTreeViewFiles::MainTreeViewFiles(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void MainTreeViewFiles::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainTreeViewFiles::dropEvent(QDropEvent *event)
{

    const QMimeData* mimeData = event->mimeData();
    PSFileSystemModel * psModel = static_cast<PSFileSystemModel*>(model());

      // Did we got some file paths
      if (mimeData->hasUrls()){
        foreach (const QUrl &url, mimeData->urls()) {
            qDebug() << "Drop it: " << url.toLocalFile();
            QFileInfo fileInfo(url.toLocalFile());

            // Import the file into our partition
            psModel->importFile(
                        url.toLocalFile().toLatin1().data(),
                        psModel->getCurrentDirectory().append("/").append(fileInfo.fileName()).toLatin1().constData()
            );
        }
      }
}
