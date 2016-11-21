#include "dialognew.h"
#include "ui_dialognew.h"
#include <QtDebug>
#include <QMessageBox>
#include <qfiledialog>
#include "../lib/libpartitionsafe/Partition.h"

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
    partitionLocFilename = fd->getSaveFileName(this, "SaveFile", "", ".vault");
    qDebug()<<"log: "<< partitionLocFilename;

    ui->textPartitionLoc->setText(partitionLocFilename);
}

void DialogNew::on_buttonBox_clicked()
{
    if(!ui->textKeyLoc->text().isEmpty()
            && !ui->textPartitionLoc->text().isEmpty()
            && !ui->textPartitionName->text().isEmpty()
            && !ui->textPartitionSize->text().isEmpty()
            && !ui->textPassword->text().isEmpty()
            && !ui->textUsername->text().isEmpty())
    {
        QByteArray baName = ui->textPartitionName->text().toLatin1();
        char* label = baName.data();
        QByteArray ba = partitionLocFilename.toLatin1();
        const char* filePath = ba.data();

        QRegExp re("\\d*");
        if(re.exactMatch(ui->textPartitionSize->text()))
        {
            int partitionSize = ui->textPartitionSize->text().toInt();
            Partition::create(label, partitionSize, filePath);
            this->accept();
        }
        else
        {
            show_error("Partition size is not a number");
        }
    }
    else
    {
        show_error("Not all fields are filled in");
    }
}
void DialogNew::show_error(const char* message)
{
    QMessageBox messageBox;
    messageBox.warning(0,"Error",message);
    messageBox.setFixedSize(500,200);
}
