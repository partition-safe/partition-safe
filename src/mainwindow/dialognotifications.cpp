#include "dialognotifications.h"
#include "ui_dialognotifications.h"

DialogNotifications::DialogNotifications(PartitionSafe *psInstance, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogNotifications), psInstance(psInstance)
{
    ui->setupUi(this);
}

DialogNotifications::~DialogNotifications()
{
    delete ui;
}
