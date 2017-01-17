#ifndef DIALOGNEWUSER_H
#define DIALOGNEWUSER_H

#include <PartitionSafe.h>
#include <QDialog>

namespace Ui {
class DialogNewUser;
}

class DialogNewUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewUser(QWidget *parent = 0, PartitionSafe* partitionSafe = 0);
    ~DialogNewUser();

private slots:
    void on_btnCreate_clicked(); // Checks input and create user

private:
    Ui::DialogNewUser *ui; // Contains uit elements

    PartitionSafe* partitionSafe; // Instance of partitionSafe
};

#endif // DIALOGNEWUSER_H
