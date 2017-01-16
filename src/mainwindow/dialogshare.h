#ifndef DIALOGSHARE_H
#define DIALOGSHARE_H

#include <PartitionSafe.h>
#include <QDialog>

namespace Ui {
class DialogShare;
}

class DialogShare : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShare(const std::string path, PartitionSafe *ps, QWidget *parent = 0);
    ~DialogShare();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogShare *ui;
    const std::string path;
    PartitionSafe *ps;
    std::vector<User*> *users;
    void load();
};

#endif // DIALOGSHARE_H
