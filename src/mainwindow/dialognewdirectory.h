#ifndef DIALOGNEWDIRECTORY_H
#define DIALOGNEWDIRECTORY_H

#include <QDialog>

namespace Ui {
class DialogNewDirectory;
}

class DialogNewDirectory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewDirectory(QWidget *parent = 0);
    ~DialogNewDirectory();
    QString dirName;

private slots:
    void on_txtDirectoryName_textChanged(const QString &arg1);

private:
    Ui::DialogNewDirectory *ui;
    bool validDirectory(QString directoryName);
    bool directoryExists(const QString directoryName);
    void enableOkButton(bool enable);
};

#endif // DIALOGNEWDIRECTORY_H
