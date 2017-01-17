#ifndef DIALOGNOTIFICATIONS_H
#define DIALOGNOTIFICATIONS_H

#include <PartitionSafe.h>
#include <QDialog>

namespace Ui {
class DialogNotifications;
}

class DialogNotifications : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNotifications(PartitionSafe *psInstance, QWidget *parent = 0);
    ~DialogNotifications();

private slots:
    void on_listNotifications_doubleClicked(const QModelIndex &index); // Hande dubbelCliked notification

private:
    Ui::DialogNotifications *ui; // Contains ui elements
    PartitionSafe *psInstance; // Instance of PartitionSafe
    std::vector<BaseNotification *> *notifications; // Vector containing norifications

    /**
     * @brief load - Retrieve notifications and add it to the list of notifications.
     */
    void load();
};

#endif // DIALOGNOTIFICATIONS_H
