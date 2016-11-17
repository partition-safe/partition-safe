#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>

namespace Ui {
class DialogNew;
}

class DialogNew : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNew(QWidget *parent = 0);
    ~DialogNew();

private slots:
    void on_buttonKeyLoc_clicked();

    void on_buttonPartitionLoc_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogNew *ui;
    QFileDialog *fd;

    QString keyLocFilename;
    QString partitionLocFilename;
};

#endif // DIALOGNEW_H
