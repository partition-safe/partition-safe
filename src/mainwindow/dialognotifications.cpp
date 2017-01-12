#include "dialognotifications.h"
#include "ui_dialognotifications.h"
#include <NotificationCentre.h>

DialogNotifications::DialogNotifications(PartitionSafe *psInstance, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogNotifications), psInstance(psInstance)
{
    ui->setupUi(this);

    // Remove [?] Button from window
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    load();
}

DialogNotifications::~DialogNotifications()
{
    delete ui;
}

void DialogNotifications::load()
{
    // Retrieve notifications
    notifications = NotificationCentre::getInstance().loadNotificationsForUser(psInstance->getUser()->id);

    // Reset listview
    ui->listNotifications->clear();

    // Add all notifications
    for(unsigned i = 0; i < notifications->size(); i++) {
        BaseNotification *notification = notifications->at(i);
        new QListWidgetItem(QString(notification->toString().c_str()), ui->listNotifications);
    }
}

void DialogNotifications::on_listNotifications_doubleClicked(const QModelIndex &index)
{
    // Call the handle
    BaseNotification *notification = notifications->at(index.row());
    notification->handle(psInstance);

    // Reload list
    load();
}
