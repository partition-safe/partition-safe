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
    QString username  = ui->txtUsername->text();
    QString password  = ui->txtPassword->text();
    QString password2 = ui->txtPassword2->text();

    // Do some validation
    if(username.size() == 0 || password.size() == 0 || password2.size() == 0){
        QMessageBox msgBox;
        msgBox.setText("Please fill in all form fields.");
        msgBox.exec();

        return;
    }
    if(password.compare(password2) != 0){
        QMessageBox msgBox;
        msgBox.setText("Both provided passwords should be eqeal.");
        msgBox.exec();

        return;
    }

    // Validation passed, continue creating the user


    // Create a new user
    User *user = User::create(username.toLatin1().data(), password.toLatin1().data());

    // Save that user to our keystore
    keyStore->saveUser(user);
}
