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

private:
    Ui::DialogNotifications *ui;
    PartitionSafe *psInstance;
};

#endif // DIALOGNOTIFICATIONS_H
