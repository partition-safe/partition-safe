#include "dialogopen.h"
#include "ui_dialogopen.h"
#include "../lib/libpartitionsafe/PartitionSafe.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

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
                && !ui->textUsername->text().isEmpty()
                && has_suffix(ui->textPartition->text(), "*.vault")
                && has_suffix(ui->textKey->text(), "*.keystore"))
        {
            if (fileExists(ui->textPartition->text())
                    && fileExists(ui->textKey->text())){
                try {
                    // Convert path to partition file to *char
                    QByteArray ba = ui->textPartition->text().toLatin1();
                    locationVault = ba.toStdString();

                    // Convert path to key store file to *char
                    ba = ui->textKey->text().toLatin1();
                    locationKeyStore = ba.toStdString();

                    // Open the partition
                    PartitionSafe* ps = new PartitionSafe;
                    ps->init(locationVault.c_str(), locationKeyStore.c_str())->open();

                    // Send accept request to end dialog.
                    this->accept();
                } catch(...) {
                    // Reset data
                    locationVault = "";
                    locationKeyStore = "";

                    show_warning("Could not open the selected vault and/or keystore.");
                }
            }else {
                show_warning("Wrong vault and/or keystore selected.");
            }
        }
        else
        {
            show_warning("Not all fields are filled in correctly");
        }
    }
}

void DialogOpen::on_buttonSelectKey_clicked()
{
    // Filter for selecting a .key file
    QStringList filters;
    filters << "Key files (*.keystore)";

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

/** Check if given path is an existing file
 *
 * @brief DialogOpen::fileExists
 * @param path - Given path to check
 * @return - true if file exists
 */
bool DialogOpen::fileExists(QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if it is a file and no directory
    return check_file.exists() && check_file.isFile();
}

/** Check if a given path has a given suffix
 *
 * @brief DialogOpen::has_suffix
 * @param filePath - Path to check if contains suffix
 * @param suffix - The suffix path sould contain
 * @return
 */
bool DialogOpen::has_suffix(const QString &filePath, const QString &suffix)
{
    // Regular expresion to check if key files end with '.key' and
    // partition files with '.vault'
    QRegExp re(suffix);
    re.setPatternSyntax(QRegExp::Wildcard);
    return re.exactMatch(filePath);
}
