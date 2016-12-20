#ifndef DIALOGOPEN_H
#define DIALOGOPEN_H

#include <QDialog>
#include <QAbstractButton>
#include <QFileDialog>
#include <PartitionSafe.h>

namespace Ui {
class DialogNotifications;
}

class DialogNotifications : public QDialog
{
    Q_OBJECT
    PartitionSafe* psInstance;

public:
    explicit DialogNotifications(PartitionSafe* psInstance, QWidget *parent = 0);
    ~DialogNotifications();

private:
    Ui::DialogNotifications *ui;

};

#endif // DIALOGOPEN_H
