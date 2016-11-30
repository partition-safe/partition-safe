#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QString>

#include <lib/PSFileSystemModel.h>

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
    void on_treeViewFiles_clicked(const QModelIndex &index);
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

private:
    Ui::MainWindow *ui;
    PSFileSystemModel *model;
    PSFileSystemModel *modelDirs;
    QStack<QString> *folderHistory, *folderForwardHistory;
    PartitionSafe* psInstance;

    void setPath();
    void importFiles();
    void importFolder();
    void exportFiles();
    void deleteFileDirectory();
    void initializeVault(const std::string vaultPath, const std::string keyStorePath);
};

#endif // MAINWINDOW_H
