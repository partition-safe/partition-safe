#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QString>
#include <QFileSystemWatcher>

#include <lib/PSFileSystemModel.h>
#include <lib/pstreefilesystemmodel.h>

namespace Ui {
class MainWindow;
}

/** The mainwindow of the application. Every Dialog and window is connected to this window.
 *
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_treeViewExplorer_doubleClicked(const QModelIndex &index); // Opens the file which is double clicked on
    void on_treeViewExplorer_selectionChanged(); // Handles if treeview has changed
    void on_buttonBack_clicked(); // Handles the back button and navigates a directory back in the treeView
    void on_buttonForward_clicked(); // Handles the forward button and navigates a directory forward in the treeeView
    void on_buttonExport_clicked(); // Handles the export button and exports a file or files
    void on_actionOpen_triggered(); // Handles the button in the menu to open a partition
    void on_actionNew_triggered(); // Handles the button in the menu to create a new partition
    void on_actionExport_triggered(); // Handles the button in the menu to export files
    void on_actionFile_triggered(); // Handles the button in the menu to import files
    void on_actionFolder_triggered(); // Handles the button in the menu to import folders
    void on_buttonImport_clicked(); // Handles the button to import files
    void on_buttonDelete_clicked(); // Handles the button to delete a file or folder from the partition
    void on_actionUser_triggered(); // Handles the button in the menu and shows a Dialog to create a new user
    void on_buttonNewDirectory_clicked(); // Handles the button to create a new directory

    void on_actionDelete_triggered(); // Handles the button the menu and deletes a file or folder
    void on_actionNew_Directory_triggered(); // Handles the button in the menu for a new directory

    void on_buttonEdit_clicked(); // Handles the edit button click to edit a file
    void on_actionRename_triggered(); // Handles the button in the menu to rename a file

    void fileChanged(const QString &); // Updates the file in the PartitionSafe if a file has been changed

    void on_buttonNotifications_clicked(); // Opens the notification center

    void on_buttonShare_clicked(); // Shares a file or folder with a user

private:
    Ui::MainWindow *ui; // Contains ui elements of this class
    PSFileSystemModel *model; // Model of the file tree
    PSTreeFileSystemModel *modelDirs; // Model of the dir tree
    QStack<QString> *folderHistory, *folderForwardHistory; // Two stacks with the history for the Forward and Backward buttons
    PartitionSafe* psInstance; // Instance of the partition safe
    QModelIndexList selectedRowsList; // Which rows has been selected
    QFileSystemWatcher* watcher; // A file watcher to detect changes in the file system
    QHash<QString, Entry*> modifiedFileList; // A list with modified files

    /** Sets the path in the status bar
     *
     * @brief setPath
     */
    void setPath();

    /** Imports one or more files in the PartitionSafe from the Dialog it opens
     *
     * @brief importFiles
     */
    void importFiles();

    /** Opens a dialog to import an folder and it contents
     *
     * @brief importFolder
     */
    void importFolder();

    /** Renames a folder
     *
     * @brief renameFileFolder
     */
    void renameFileFolder();

    /** Creates a new PartitionSafe instance to store files
     *
     * @brief initializeVault
     * @param vaultPath - Path to where the vault is going to be stored
     * @param keyStorePath - Path to where the keystore is going to be stored
     * @param username - Username of the user
     * @param password - Password which is going to be used to create the hash
     */
    void initializeVault(const std::string vaultPath, const std::string keyStorePath, const std::string username, const std::string password);

    /** Creates a new directory in the system of the PartitionSafe
     *
     * @brief makeDir
     * @param path - Path with the new directory name
     */
    void makeDir(QString path);

    /** Export selected files or folders from the partion
     *
     * @brief exportFiles
     */
    void exportFiles();

    /** Export given file/folder (sourcePath) to selected location (destinationDir)
     *
     * @brief exportFile
     * @param sourcePath - File or folder to export
     * @param destinationPath - destination dir to export to.
     */
    void exportFile(QString sourcePath, QString destinationPath);

    /** Delete the slected files and directories.
     *
     * @brief deleteFileDirectory
     */
    void deleteFileDirectory();

    /** Gets the notifications from the keystore and shows a button in the MainWindow
     *
     * @brief setNotifications
     */
    void setNotifications();

    /** Shows a Dialog to make a new directory
     *
     * @brief newDirectory
     */
    void newDirectory();
};

#endif // MAINWINDOW_H
