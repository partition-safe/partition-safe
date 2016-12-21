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

    // Enable if Directory name is valid. else Disable.
    enableOkButton(validDirectory(ui->txtDirectoryName->text()));
}

DialogNewDirectory::~DialogNewDirectory()
{
    delete ui;
}

bool DialogNewDirectory::validDirectory(QString directoryName)
{
    // Regex to check if directoryName not contains forbidden charactes (mainly windows).
    // ^        = Start of string
    // (?!\\s)+ = Not (starting with) one or more spaces.
    // ^        = Start of string (again)
    // [^ ]+    = Not containing one or more characters within the brackets
    // \\\\     = For \ character
    // \        = Esc character.
    // $        = End of string
    QRegExp re("^(?!\\s)+^[^<>\\\\/:|\"\*\?\.]+$");
    return re.exactMatch(directoryName);
}

void  DialogNewDirectory::enableOkButton(bool enable)
{
    // Check if valid dirictory and set OK enable/disable OK button
    QPushButton *button = ui->buttonBoxNewDir->button(QDialogButtonBox::Ok);
    // Enable or disable OK button
    button->setEnabled(enable);
    // If directory name allowed save directory name.
    if(enable) dirName = ui->txtDirectoryName->text();
}

void DialogNewDirectory::on_txtDirectoryName_textChanged(const QString &dirName)
{
    // Check if input is valid and enable/diable the OK button
    bool validDir = validDirectory(dirName);
    enableOkButton(validDir);

    if(!validDir)ui->lblWarning->setText("A Directory name can't be empty or contain any of the following characters:  \\ / : * ? \" < > | .");
    else ui->lblWarning->setText(""); // Clear the label
}
