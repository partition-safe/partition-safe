#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogopen.h"
#include "dialognew.h"

#include <QDirModel>
#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    folderHistory(new QStack<QString>),
    folderForwardHistory(new QStack<QString>)
{
    // Setup the UI
    ui->setupUi(this);

    // Create the partition safe instance
    psInstance = new PartitionSafe();

    // Temporary
    initializeVault("/tmp/marc.vault", "/tmp/marc.keystore");

    // Show path in status bar
    this->setPath();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete folderHistory;
    delete folderForwardHistory;
    delete model;
    delete modelDirs;
}

void MainWindow::on_treeViewExplorer_doubleClicked(const QModelIndex &index)
{
    // We should get the selected item
    Entry* item = model->getFile(index);

    // Only continue if it's a directory
    if(!item->isDirectory()) return;

    // Get the path
    QString path = QString(item->getFullPath().c_str());

    // Add history to stack
    folderHistory->append(model->getCurrentDirectory());

    // Set directory
    model->setCurrentDirectory(path);

    // clear forward if it doesn't contain dir
    // else pop from ForwardHistory
    if (!folderForwardHistory->contains(path)) folderForwardHistory->clear();
    else folderForwardHistory->pop();

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_treeViewFiles_clicked(const QModelIndex &index)
{
//    this->on_treeViewExplorer_doubleClicked(index);
}

void MainWindow::on_buttonBack_clicked()
{
    // More than only the home item
    if(folderHistory->size() > 0)
    {
        // Get the first item of the stack
        QString dir = folderHistory->pop();

        // Add to folderForwardHistory
        folderForwardHistory->append(dir);

        // Set directory
        model->setCurrentDirectory(dir);
    }

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_buttonForward_clicked()
{
    // If there is a Forward
    if(folderForwardHistory->size() > 0)
    {
        // Get first item of the forward stack
        QString dir = folderForwardHistory->pop();

        // Append dir to folderHistory
        folderHistory->append(dir);

        // Set directory
        model->setCurrentDirectory(dir);
    }

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_actionOpen_triggered()
{
    DialogOpen *open = new DialogOpen(this);

    open->exec();
}

void MainWindow::on_actionNew_triggered()
{
    DialogNew *newDialog = new DialogNew(this);

    newDialog->exec();
}

void MainWindow::on_buttonExport_clicked()
{
    exportFiles();
}

void MainWindow::on_buttonImport_clicked()
{
    importFiles();
}

void MainWindow::on_actionFolder_triggered()
{
    importFolder();
}

void MainWindow::on_actionFile_triggered()
{
    importFiles();
}

void MainWindow::on_actionExport_triggered()
{
    exportFiles();
}

void MainWindow::importFiles()
{
    QFileDialog qFile;
    // Allow selecting of multiple files
    qFile.setFileMode(QFileDialog::Directory);
    // Open File dialog
    qFile.exec();

    qDebug() << qFile.directory().entryList();
    foreach (QString filePath, qFile.selectedFiles()) {
        qDebug() << filePath;

        // TODO: import file from filePath
    }
}

void MainWindow::importFolder(){
        qDebug() << "importFolder() called";
}

void MainWindow::exportFiles()
{
    QModelIndexList selectedRowsList = ui->treeViewExplorer->selectionModel()->selectedRows();
    foreach (QModelIndex index, selectedRowsList)
    {
//        qDebug() << modelDirs->filePath(index);

        // TODO: Export selected file.
    }
}

void MainWindow::initializeVault(const QString vaultPath, const QString keyStorePath)
{
    // Convert names
    const char *cVaultPath = vaultPath.toStdString().c_str();
    const char *cKeyStorePath = keyStorePath.toStdString().c_str();

    // Setup vault
    psInstance->init(cVaultPath, cKeyStorePath);
    psInstance->open();

    // Create file system models
    model = new PSFileSystemModel(this, psInstance);
    modelDirs = new PSFileSystemModel(this, psInstance);

    // Set models in views
    ui->treeViewExplorer->setModel(model);
    ui->treeViewFiles->setModel(modelDirs);
}

void MainWindow::setPath()
{
    // Show message
    ui->statusBar->showMessage(model->getCurrentDirectory());

    // At last item? Disable back button.
    ui->buttonBack->setEnabled(folderHistory->size() > 0);

    // At last forward item? Disable forward button.
    ui->buttonForward->setEnabled(folderForwardHistory->size() > 0);
}
