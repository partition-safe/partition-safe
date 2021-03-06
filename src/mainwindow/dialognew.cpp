#include "dialognew.h"
#include "ui_dialognew.h"

#include <PartitionSafe.h>
#include <QMessageBox>

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
    keyLocFilename = fd->getSaveFileName(this, "SaveFile", "", "Key files (*.keystore)");

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
        // Partition size check
        QRegExp re("\\d*");

        // Check per field
        if(ui->textKeyLoc->text().isEmpty())
        {
            show_warning("No keystore location selected.");
            return;
        }
        else if(ui->textPartitionLoc->text().isEmpty())
        {
            show_warning("No vault location selected.");
            return;
        }
        else if(ui->textPartitionName->text().isEmpty())
        {
            show_warning("No partion name inserted.");
            return;
        }
        else if(ui->textPartitionSize->text().isEmpty())
        {
            show_warning("No partition size inserted.");
            return;
        }
        else if(ui->textPassword->text().isEmpty())
        {
            show_warning("No password inserted.");
            return;
        }
        else if(ui->textUsername->text().isEmpty())
        {
            show_warning("No username inserted.");
            return;
        }
        else if(!has_suffix(ui->textPartitionLoc->text(), "*.vault"))
        {
            show_warning("No valid vault extension (use .vault).");
            return;
        }
        else if(!has_suffix(ui->textKeyLoc->text(), "*.keystore"))
        {
            show_warning("No valid keystore extension (use .keystore).");
            return;
        }
        else if(!re.exactMatch(ui->textPartitionSize->text()))
        {
            show_warning("No valid partition size inserted.");
            return;
        }

        // Convert name and path to partition file to *char
        QByteArray baName = ui->textPartitionName->text().toLatin1();
        const char *label = baName.data();
        QByteArray baPartLoc = ui->textPartitionLoc->text().toLatin1();
        const char *filePath = baPartLoc.data();
        QByteArray baKeyLoc = ui->textKeyLoc->text().toLatin1();
        const char *keyPath = baKeyLoc.data();
        QByteArray baUsername = ui->textUsername->text().toLatin1();
        const char *username = baUsername.data();
        QByteArray baPassword = ui->textPassword->text().toLatin1();
        const char *password = baPassword.data();

        int partitionSize = ui->textPartitionSize->text().toInt();

        // multiply the partition size
        switch(ui->comboSizeType->currentIndex()){
            case 3: // KB
                partitionSize *= 1024;
            break;
            case 2: // MB
                partitionSize *= 1048576;
            break;
            case 1: // GB
                partitionSize *= 1073741824;
            break;
            case 0: // TB
                partitionSize *= 1099511627776;
            break;
        }

        try {
            // Create a partition
            PartitionSafe* ps = new PartitionSafe();
            ps->create(filePath, keyPath, label, partitionSize, username, password);
            delete ps;
        } catch(...) {
            show_warning("Could not create partition.");
            return;
        }

        // Send accept request to end dialog.
        this->accept();
    }
}

void DialogNew::show_warning(const char* message)
{
    QMessageBox messageBox;
    messageBox.warning(0,"Warning",message);
}

bool DialogNew::has_suffix(const QString &filePath, const QString &suffix)
{
    // Regular expresion to check if key files end with '.key' and
    // partition files with '.vault'
    QRegExp re(suffix);
    re.setPatternSyntax(QRegExp::Wildcard);
    return re.exactMatch(filePath);
}
