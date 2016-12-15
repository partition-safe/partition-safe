#include "dialognewdirectory.h"
#include "ui_dialognewdirectory.h"
#include <QPushButton>

DialogNewDirectory::DialogNewDirectory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDirectory)
{
    ui->setupUi(this);

    // Remove [?] Button from window
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Enable or disable OK butotn
    enableOkButton(validDirectory(ui->txtDirectoryName->text()));
}

DialogNewDirectory::~DialogNewDirectory()
{
    delete ui;
}

bool DialogNewDirectory::validDirectory(QString directoryName){
    //Todo: add regexp to check directory name
    QRegExp re("^[^<>\\\\/:|\"\*\?\.]+$");
    return re.exactMatch(directoryName);
}

void  DialogNewDirectory::enableOkButton(bool enable){
    // Check if valid dirictory and set OK enable/disable OK burron
    QPushButton *button = ui->buttonBoxNewDir->button(QDialogButtonBox::Ok);
    button->setEnabled(enable);
    if(enable) dirName = ui->txtDirectoryName->text();
}

void DialogNewDirectory::on_txtDirectoryName_textChanged(const QString &dirName)
{
    // Check if input is valid and enable/diable the OK button
    enableOkButton(validDirectory(dirName));
}
