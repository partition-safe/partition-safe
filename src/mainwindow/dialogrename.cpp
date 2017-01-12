#include "dialogrename.h"
#include "ui_dialogrename.h"
#include <QPushButton>

DialogRename::DialogRename(QWidget *parent, QString name) :
    QDialog(parent),
    fileName(name),
    ui(new Ui::DialogRename)
{
    ui->setupUi(this);

    // Remove [?] Button from window
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Enable if Directory name is valid. else Disable.
    enableOkButton(validName(ui->txtRename->text()));

    // Set input to file/folder name
    ui->txtRename->setText(fileName);
}

DialogRename::~DialogRename()
{
    delete ui;
}

bool DialogRename::validName(QString name)
{
    // Regex to check if directoryName not contains forbidden charactes (mainly windows).
    // ^        = Start of string
    // (?!\\s)+ = Not (starting with) one or more spaces.
    // ^        = Start of string (again)
    // [^ ]+    = Not containing one or more characters within the brackets
    // \\\\     = For \ character
    // \        = Esc character.
    // $        = End of string
    QRegExp re("^(?!\\s)+^[^<>\\\\/:|\"\*\?]+$");
    // Return true if directory name matching regex
    return re.exactMatch(name);
}

void  DialogRename::enableOkButton(bool enable)
{
    // Check if valid dirictory and set OK enable/disable OK button
    QPushButton *button = ui->buttonBoxRename->button(QDialogButtonBox::Ok);
    // Enable or disable OK button
    button->setEnabled(enable);
    // If directory name allowed save directory name.
    if(enable) fileName = ui->txtRename->text();
}

void DialogRename::on_txtRename_textChanged(const QString &dirName)
{
    // Check if input is valid and enable/diable the OK button
    bool validDir = validName(dirName);
    enableOkButton(validDir);

    if(!validDir)ui->lblRenameWarning->setText("A Directory name can't be empty or contain any of the following characters:  \\ / : * ? \" < > | .");
    else ui->lblRenameWarning->setText(""); // Clear the label
}
