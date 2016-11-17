#include "dialognew.h"
#include "ui_dialognew.h"
#include <QtDebug>
#include <qfiledialog>

DialogNew::DialogNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNew)
{
    ui->setupUi(this);
}

DialogNew::~DialogNew()
{
    delete ui;
}
void DialogNew::on_buttonKeyLoc_clicked()
{
    keyLocFilename = fd->getSaveFileName(this, "SaveFile", "", ".key");
    qDebug()<<"log: "<< keyLocFilename;

    ui->textKeyLoc->setText(keyLocFilename);
}

void DialogNew::on_buttonPartitionLoc_clicked()
{
    partitionLocFilename = fd->getSaveFileName(this, "SaveFile", "", ".partition");
    qDebug()<<"log: "<< partitionLocFilename;

    ui->textPartitionLoc->setText(partitionLocFilename);
}

void DialogNew::on_buttonBox_accepted()
{
    //Just for testing
    QFile kfile(keyLocFilename);
    kfile.open(QIODevice::WriteOnly);
    kfile.write("key file");
    kfile.flush();
    kfile.close();

    QFile pfile(partitionLocFilename);
    pfile.open(QIODevice::WriteOnly);
    pfile.write("partition file");
    pfile.flush();
    pfile.close();
}
