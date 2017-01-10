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
    void on_listNotifications_doubleClicked(const QModelIndex &index);

private:
    Ui::DialogNotifications *ui;
    PartitionSafe *psInstance;
    std::vector<BaseNotification *> *notifications;
    void load();
};

#endif // DIALOGNOTIFICATIONS_H
