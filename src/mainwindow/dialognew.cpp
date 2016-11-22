#include "dialognew.h"
#include "ui_dialognew.h"
#include <QMessageBox>
#include "../lib/libpartitionsafe/Partition.h"

DialogNew::DialogNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNew)
{
    ui->setupUi(this);
    // Remove [?] Button from window
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Accept only positive numbers for the size in bytes
    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("\\d*"), this);
    ui->textPartitionSize->setValidator(rxv);
}

DialogNew::~DialogNew()
{
    delete ui;
}
void DialogNew::on_buttonKeyLoc_clicked()
{
    // Get location to save the key file
    keyLocFilename = fd->getSaveFileName(this, "SaveFile", "", "Key files (*.key)");

    // Set path to key file in textbox
    ui->textKeyLoc->setText(keyLocFilename);
}

void DialogNew::on_buttonPartitionLoc_clicked()
{
    // Get location to save the partition file
    partitionLocFilename = fd->getSaveFileName(this, "SaveFile", "", "Partition files (*.vault)");

    // Set path to partition file in textbox
    ui->textPartitionLoc->setText(partitionLocFilename);
}

void DialogNew::on_buttonBox_clicked(QAbstractButton *button)
{
    // Check if the [OK] button is clicked
    if(button== ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        // Check if all fields are filled in
        if(!ui->textKeyLoc->text().isEmpty()
                && !ui->textPartitionLoc->text().isEmpty()
                && !ui->textPartitionName->text().isEmpty()
                && !ui->textPartitionSize->text().isEmpty()
                && !ui->textPassword->text().isEmpty()
                && !ui->textUsername->text().isEmpty())
        {
            // Convert name and path to partition file to *char
            QByteArray baName = ui->textPartitionName->text().toLatin1();
            char *label = baName.data();
            QByteArray ba = partitionLocFilename.toLatin1();
            const char *filePath = ba.data();

            // Check if partition size is a number
            QRegExp re("\\d*");
            if(re.exactMatch(ui->textPartitionSize->text()))
            {
                // Creat a partition
                int partitionSize = ui->textPartitionSize->text().toInt();
                Partition::create(label, partitionSize, filePath);
                // Send accept request to end dialog.
                this->accept();
            }
            else {
                show_warning("Partition size is not a number");
            }
        }
        else {
            show_warning("Not all fields are filled in");
        }
    }
}

/** Display warning message.
 *
 * @brief DialogOpen::show_warning
 * @param message - Given message to display
 */
void DialogNew::show_warning(const char* message)
{
    QMessageBox messageBox;
    messageBox.warning(0,"Warning",message);
}
