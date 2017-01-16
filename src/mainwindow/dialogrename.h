#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H

#include <QDialog>

namespace Ui {
class DialogRename;
}

class DialogRename : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRename(QWidget *parent = 0, QString name = "");
    ~DialogRename();
    QString fileName;

private slots:
    void on_txtRename_textChanged(const QString &arg1);

private:
    Ui::DialogRename *ui;

    /** Check if directory/file name is valid. Uses regex to check the directory name.
   *
   * @brief validDirectory
   * @param directoryName - name of the directory to check
   * @return - True if valid directory name
   */
    bool validName(QString name);

    /** Enabels the OK (submit) button and saves the name name in the var fileName.
   * @brief enableOkButton
   * @param enable - True or False.
   */
    void enableOkButton(bool enable);
};

#endif // DIALOGRENAME_H
