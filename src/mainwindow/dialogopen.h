#ifndef DIALOGOPEN_H
#define DIALOGOPEN_H

#include <QDialog>
#include <QAbstractButton>
#include <QFileDialog>

namespace Ui {
class DialogOpen;
}

class DialogOpen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpen(QWidget *parent = 0);
    ~DialogOpen();
    std::string locationVault;
    std::string locationKeyStore;
    std::string username;
    std::string password;

private slots:
    void on_buttonSelectPartition_clicked();
    void on_buttonSelectKey_clicked();
    void on_buttonBoxDialogOpen_clicked(QAbstractButton *button);

private:
    Ui::DialogOpen *ui;
    void show_warning(const char*);
    bool fileExists(QString);
    bool has_suffix(const QString &filePath, const QString &suffix);

};

#endif // DIALOGOPEN_H
