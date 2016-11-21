#include "dialogopen.h"
#include "ui_dialogopen.h"
#include "../lib/libpartitionsafe/Partition.h"

#include <QtDebug>
#include <QFileDialog>

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

void DialogOpen::on_buttonSelectKey_clicked()
{
    qDebug()<<"SlectKey";
    QStringList filters;
    filters << "Key files (*.key)";

    QFileDialog qFile;
    qFile.setNameFilters(filters);
    qFile.exec();

    ui->textKey->setText(qFile.selectedFiles()[0]);
}

void DialogOpen::on_buttonSelectPartition_clicked()
{
    qDebug()<<"SlectPartition";
    QStringList filters;
    filters << "Partition files (*.vault)";

    QFileDialog qFile;
    qFile.setNameFilters(filters);
    qFile.exec();

    ui->textPartition->setText(qFile.selectedFiles()[0]);

    QByteArray ba = qFile.selectedFiles()[0].toLatin1();
    const char *c_fileLocation = ba.data();
    Partition::open(c_fileLocation);
}
