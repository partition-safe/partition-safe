#include "dialogopen.h"
#include "ui_dialogopen.h"
#include "../lib/libpartitionsafe/Partition.h"

#include <QFileDialog>
#include <QMessageBox>

DialogOpen::DialogOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpen)
{
    ui->setupUi(this);

    // Remove [?] Button from window
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

DialogOpen::~DialogOpen()
{
    delete ui;
}

void DialogOpen::on_buttonBoxDialogOpen_clicked(QAbstractButton *button)
{
    // Check if the [OK] button is clicked
    if(button == ui->buttonBoxDialogOpen->button(QDialogButtonBox::Ok))
    {
        // Check if all fields are filled in
        if(!ui->textKey->text().isEmpty()
                && !ui->textPartition->text().isEmpty()
                && !ui->textPassword->text().isEmpty()
                && !ui->textUsername->text().isEmpty())
        {
            // Convert path to partition file to *char
            QByteArray ba = ui->textPartition->text().toLatin1();
            const char *c_fileLocation = ba.data();
            // Open the partition
            Partition::open(c_fileLocation);
            // Send accept request to end dialog.
            this->accept();
        } else{
            show_warning("Not all fields are filled in");
        }
    }
}

void DialogOpen::on_buttonSelectKey_clicked()
{
    // Filter for selecting a .key file
    QStringList filters;
    filters << "Key files (*.key)";

    // Create new FileDialog, Set the filters and execute the Dialog
    // Select a key file.
    QFileDialog qFile;
    qFile.setNameFilters(filters);
    qFile.exec();

    // Set path to the key file if not a file is selected.
    if(!qFile.selectedFiles().empty()) ui->textKey->setText(qFile.selectedFiles()[0]);
}

void DialogOpen::on_buttonSelectPartition_clicked()
{
    // Filter for selecting a .vault file
    QStringList filters;
    filters << "Partition files (*.vault)";

    // Create new FileDialog, Set the filters and execute the Dialog
    // Slect a vault file.
    QFileDialog qFile;
    qFile.setNameFilters(filters);
    qFile.exec();

    // Set path to partition file in textbox if not empty.
    if(!qFile.selectedFiles().empty()) ui->textPartition->setText(qFile.selectedFiles()[0]);
}

/** Display warning message.
 *
 * @brief DialogOpen::show_warning
 * @param message - Given message to display
 */
void DialogOpen::show_warning(const char* message)
{
    QMessageBox messageBox;
    messageBox.warning(0,"Warning",message);
}
