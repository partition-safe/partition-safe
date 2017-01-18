#include "dialogshare.h"
#include "ui_dialogshare.h"

#include <NotificationCentre.h>
#include <notification/DirectoryInvitationNotification.h>

DialogShare::DialogShare(const std::string path, PartitionSafe *ps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShare), path(path), ps(ps)
{
    users = new std::vector<User*>();
    ui->setupUi(this);
    load();
}

DialogShare::~DialogShare()
{
    delete users;
    delete ui;
}

void DialogShare::load()
{
    // Get the current user
    User *currentUser = ps->getUser();

    // Retrieve users
    users->clear();
    std::vector<User *> *tUsers = new std::vector<User *>();
    this->ps->getKeyStore()->getUsers(&tUsers);
    for(unsigned i = 0; i < tUsers->size(); i++) {
        User *user = tUsers->at(i);
        if(user->id != currentUser->id) {
            users->push_back(user);
        }
    }

    // Reset listview
    ui->listUsers->clear();

    // Add all notifications
    for(unsigned i = 0; i < users->size(); i++) {
        User *user = users->at(i);
        new QListWidgetItem(QString(user->username), ui->listUsers);
    }
}

void DialogShare::on_buttonBox_accepted()
{
    // Iterate over selection
    for(unsigned i = 0; i < ui->listUsers->selectionModel()->selectedRows().size(); i++) {
        // Retrieve the user
        QModelIndex item = ui->listUsers->selectionModel()->selectedRows().at(i);
        User *user = users->at(item.row());

        // Create notification
        DirectoryInvitationNotification *notif = DirectoryInvitationNotification::create(ps->getUser(), user, path, "");

        // Save the notification
        ps->getNotificationCentre()->saveNotification(notif);
    }
}
