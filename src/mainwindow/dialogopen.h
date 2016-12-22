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
    /** On button click for selecting a Partition.
     *
     * @brief on_buttonSelectPartition_clicked
     */
    void on_buttonSelectPartition_clicked();

    /** On button click for selecting a keystore.
     *
     * @brief on_buttonSelectKey_clicked
     */
    void on_buttonSelectKey_clicked();

    /** On a butotn click in the buttonBox (buttonBoxDialogOpen)
      *
     * @brief on_buttonBoxDialogOpen_clicked
     * @param button - The button clicked in the buttonBox.
     */
    void on_buttonBoxDialogOpen_clicked(QAbstractButton *button);

private:
    Ui::DialogOpen *ui;

    /** Display warning message.
     *
     * @brief DialogOpen::show_warning
     * @param message - Given message to display
     */
    void show_warning(const char*);

    /** Check if given path is an existing file
     *
     * @brief DialogOpen::fileExists
     * @param path - Given path to check
     * @return - true if file exists
     */
    bool fileExists(QString);

    /** Check if a given path has a given suffix
     *
     * @brief DialogOpen::has_suffix
     * @param filePath - Path to check if contains suffix
     * @param suffix - The suffix path sould contain
     * @return
     */
    bool has_suffix(const QString &filePath, const QString &suffix);

};

#endif // DIALOGOPEN_H
