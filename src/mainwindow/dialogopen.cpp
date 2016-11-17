#include "dialogopen.h"
#include "ui_dialogopen.h"

#include <QtDebug>

DialogOpen::DialogOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpen)
{
    ui->setupUi(this);
}

DialogOpen::~DialogOpen()
{
    delete ui;
}

void DialogOpen::on_buttonBoxDialogOpen_accepted()
{
    qDebug()<<"Accepted";
}

void DialogOpen::on_buttonBoxDialogOpen_rejected()
{
    qDebug()<<"Rejected";
}
