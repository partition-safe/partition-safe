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
    void on_buttonBox_accepted(); // Set notification for selected users

private:
    Ui::DialogShare *ui; // Contains ui elements
    const std::string path; // Path to file or folder to share
    PartitionSafe *ps; // PartitionSafe instance
    std::vector<User*> *users; // Vector of users


    /**
     * @brief load - Get and set list of users to potentialy share with
     */
    void load();
};

#endif // DIALOGSHARE_H
