#ifndef MAINTREEVIEWFILES_H
#define MAINTREEVIEWFILES_H

#include <QTreeView>
#include <PartitionSafe.h>

QT_MODULE(Gui)

/** This class handels the drag and drop functionalitie
 *
 * @brief The MainTreeViewFiles class
 */
class MainTreeViewFiles : public QTreeView
{
    Q_OBJECT
public:
    /** Contructor of the object, expects the parent
     *
     * @brief MainTreeViewFiles
     * @param parent
     */
    explicit MainTreeViewFiles(QWidget *parent = 0);

protected:
    /** Handels the drag event and accepts it
     *
     * @brief dragEnterEvent
     * @param event
     */
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

    /** Gets called when a file is dropped in the FileTree in the mainwindow
     *
     * @brief dropEvent
     * @param event
     */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // MAINTREEVIEWFILES_H
