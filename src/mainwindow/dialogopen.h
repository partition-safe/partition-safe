#ifndef DIALOGOPEN_H
#define DIALOGOPEN_H

#include <QDialog>

namespace Ui {
class DialogOpen;
}

class DialogOpen : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpen(QWidget *parent = 0);
    ~DialogOpen();

private slots:
    void on_buttonSelectPartition_clicked();
    void on_buttonSelectKey_clicked();

private:
    Ui::DialogOpen *ui;

};

#endif // DIALOGOPEN_H
