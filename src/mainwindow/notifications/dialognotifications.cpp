#include "dialognotifications.h"
#include "ui_dialogopen.h"

#include <NotificationCentre.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DialogNotifications::DialogNotifications(PartitionSafe* psInstance, QWidget *parent) :
    QDialog(parent), psInstance(psInstance), ui(new Ui::DialogOpen)
{
    ui->setupUi(this);

    // Retrieve notifications
    std::vector<BaseNotification *> *notifications = NotificationCentre::getInstance().loadNotificationsForUser(psInstance->getUser()->id);

    // Add items
    for(unsigned i = 0; i < notifications->size(); i++) {
        ui->listNotifications->addItem(notifications->at(i)->toString());
    }
}

DialogOpen::~DialogOpen()
{
    delete ui;
}
