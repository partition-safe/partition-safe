#include "dialognewuser.h"
#include "ui_dialognewuser.h"
#include <QDebug>
#include <QMessageBox>

DialogNewUser::DialogNewUser(QWidget *parent, PartitionSafe* partitionSafe) :
    QDialog(parent),
    ui(new Ui::DialogNewUser),
    partitionSafe(partitionSafe)
{
    ui->setupUi(this);
    setSizeGripEnabled(false);
}

DialogNewUser::~DialogNewUser()
{
    delete ui;
}

void DialogNewUser::on_btnCreate_clicked()
{
    KeyStore *keyStore = partitionSafe->getKeyStore();

    // Retrieve user credentials
    QByteArray baUsername = ui->txtUsername->text().toLatin1();
    const char *username = baUsername.data();
    QByteArray baPassword = ui->txtPassword->text().toLatin1();
    const char *password = baPassword.data();
    QByteArray baPassword2 = ui->txtPassword2->text().toLatin1();
    const char *password2 = baPassword2.data();

    // Do some validation
    if(strlen(username) == 0 || strlen(password) == 0 || strlen(password2) == 0){
        QMessageBox msgBox;
        msgBox.setText("Please fill in all form fields.");
        msgBox.exec();

        return;
    }
    if(strlen(password) < 8){
        QMessageBox msgBox;
        msgBox.setText("The password needs to be at least 8 characters.");
        msgBox.exec();

        return;
    }
    if(strcmp(password, password2) != 0){
        QMessageBox msgBox;
        msgBox.setText("Both provided passwords should be eqeal.");
        msgBox.exec();
        return;
    }

    try {
        // Validation passed, continue creating the user
        User *user;
        Key *key;
        partitionSafe->createUser(username, password, &user, &key);

        QMessageBox msgBox;
        msgBox.setText("The user has been created");
        msgBox.exec();

        close();

    } catch(char const *error) {
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
    }
}
