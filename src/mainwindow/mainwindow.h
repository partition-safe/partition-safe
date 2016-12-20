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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_treeViewExplorer_doubleClicked(const QModelIndex &index);
    void on_treeViewExplorer_selectionChanged();
    void on_buttonBack_clicked();
    void on_buttonForward_clicked();
    void on_buttonExport_clicked();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionExport_triggered();
    void on_actionFile_triggered();
    void on_actionFolder_triggered();
    void on_buttonImport_clicked();
    void on_buttonDelete_clicked();

    void fileChanged(const QString &);

    void on_buttonNewDirectory_clicked();
    void on_treeViewExplorer_viewportEntered();

private:
    Ui::MainWindow *ui;
    PSFileSystemModel *model;
    PSTreeFileSystemModel *modelDirs;
    QStack<QString> *folderHistory, *folderForwardHistory;
    PartitionSafe* psInstance;
    QModelIndexList selectedRowsList;
    QFileSystemWatcher* watcher;
    QHash<QString, Entry*> modifiedFileList;

    void setPath();
    void importFiles();
    void importFolder();

    /** Export selected files or folders from the partion
     *
     * @brief exportFiles
     */
    void exportFiles();

    /** Export given file/folder (sourcePath) to selected location (destinationDir)
     *
     * @brief exportFile
     * @param sourcePath - File or folder to export
     * @param destinationDir - destination te export to
     * @param destinationPath - Full destination path including file/folder name to export.
     */
    void exportFile(QString sourcePath, QString destinationDir, QString destinationPath);

    void initializeVault(const std::string vaultPath, const std::string keyStorePath, const std::string username, const std::string password);

    /** Delete the slected files and directories
     *
     * @brief deleteFileDirectory
     */
    void deleteFileDirectory();
};

#endif // MAINWINDOW_H
