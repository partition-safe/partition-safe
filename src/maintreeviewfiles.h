#ifndef MAINTREEVIEWFILES_H
#define MAINTREEVIEWFILES_H

#include <QTreeView>
#include <PartitionSafe.h>

QT_MODULE(Gui)

class MainTreeViewFiles : public QTreeView
{
    Q_OBJECT
public:
    explicit MainTreeViewFiles(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // MAINTREEVIEWFILES_H
