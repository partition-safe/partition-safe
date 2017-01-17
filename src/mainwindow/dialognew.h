#ifndef DIALOGNEW_H
#define DIALOGNEW_H

#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QAbstractButton>

namespace Ui {
class DialogNew;
}

/**
 * Dialog to create a new PartitionSafe
 */
class DialogNew : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNew(QWidget *parent = 0);
    ~DialogNew();

private slots:
    void on_buttonKeyLoc_clicked(); // Opens dialog for the keystore location
    void on_buttonPartitionLoc_clicked(); // Opens dialog for the vault location
    void on_buttonBox_clicked(QAbstractButton *button); // Handles the OK or Cancel buttons and checks if the input is correct

private:
    Ui::DialogNew *ui; // Contains ui elements
    QFileDialog *fd; // FileDialog to select a file location to store the keystore and vault
    QString keyLocFilename; // Location of the keystore
    QString partitionLocFilename; // location of the vault

    /** Shows a warning when a criteria has not been met
     *
     * @brief show_warning
     * @param message - shows which criteria has not been met
     */
    void show_warning(const char* message);

    /** Checks input if the input ends with a .key or .vault
     *
     * @brief has_suffix
     * @param filePath - filepath with the file
     * @param suffix - which suffix to check for
     * @return
     */
    bool has_suffix(const QString &filePath, const QString &suffix);
};

#endif // DIALOGNEW_H
