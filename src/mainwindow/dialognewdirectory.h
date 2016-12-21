#ifndef DIALOGNEWDIRECTORY_H
#define DIALOGNEWDIRECTORY_H

#include <QDialog>

namespace Ui {
class DialogNewDirectory;
}

class DialogNewDirectory : public QDialog {
    Q_OBJECT

public:
    explicit DialogNewDirectory(QWidget *parent = 0);
    ~DialogNewDirectory();
    QString dirName;

private slots:
    void on_txtDirectoryName_textChanged(const QString &arg1);

private:
    Ui::DialogNewDirectory *ui;

    /** Check if directory name is valid. Uses regex to check the directory name.
   *
   * @brief validDirectory
   * @param directoryName - name of the directory to check
   * @return - True if valid directory name
   */
    bool validDirectory(QString directoryName);

    /** Enabels the OK (submit) button and saves the directory name in the var dirName.
   * @brief enableOkButton
   * @param enable - True or False.
   */
    void enableOkButton(bool enable);
};

#endif // DIALOGNEWDIRECTORY_H
