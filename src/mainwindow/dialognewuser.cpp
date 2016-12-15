#include "dialognewuser.h"
#include "ui_dialognewuser.h"

DialogNewUser::DialogNewUser(QWidget *parent, PartitionSafe* partitionSafe) :
    QDialog(parent),
    ui(new Ui::DialogNewUser),
    partitionSafe(partitionSafe)
{
    ui->setupUi(this);
}

DialogNewUser::~DialogNewUser()
{
    delete ui;
}
