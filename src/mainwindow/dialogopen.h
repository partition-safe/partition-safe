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

private:
    Ui::DialogOpen *ui;
    void on_buttonBoxDialogOpen_accepted();
    void on_buttonBoxDialogOpen_rejected();
};

#endif // DIALOGOPEN_H
