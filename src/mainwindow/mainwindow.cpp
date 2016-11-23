#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogopen.h"
#include "dialognew.h"

#include <QFileSystemModel>
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

    // Create file system models
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::homePath());
    modelDirs = new QFileSystemModel(this);
    modelDirs->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    // Set models in views
    ui->treeViewExplorer->setModel(model);
    ui->treeViewExplorer->setRootIndex(model->index(QDir::homePath()));
    ui->treeViewFiles->setModel(modelDirs);
    ui->treeViewFiles->hideColumn(3);
    ui->treeViewFiles->hideColumn(2);
    ui->treeViewFiles->hideColumn(1);

    // Add to stack
    folderHistory->append(QDir::homePath());

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
    // Get directory
    QString dir(model->filePath(index));

    // clear forward if it doesn't contain dir
    // else pop from ForwardHistory
    if (!folderForwardHistory->contains(dir)) folderForwardHistory->clear();
    else folderForwardHistory->pop();

    // Add to stack
    folderHistory->append(dir);

    // Set root index
    ui->treeViewExplorer->setRootIndex(model->index(dir));

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_treeViewFiles_clicked(const QModelIndex &index)
{
    this->on_treeViewExplorer_doubleClicked(index);
}

void MainWindow::on_buttonBack_clicked()
{
    // Track dir
    QString dir;

    // More than only the home item
    if(folderHistory->size() > 0)
    {
        // Get the first item of the stack
        dir = folderHistory->pop();

        // Add to folderForwardHistory
        folderForwardHistory->append(dir);

        // Is it the current directory?
        if(folderHistory->size() > 0 && ui->treeViewExplorer->rootIndex() == model->index(dir)) {
            // Pop another
            dir = folderHistory->last();
        }

        // Set root index
        ui->treeViewExplorer->setRootIndex(model->index(dir));
    }

    // History empty?
    if(folderHistory->size() == 0)
    {
        // Append the last dir as it must be the starting point
        folderHistory->append(dir);
    }

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_buttonForward_clicked()
{
    // Track dir
    QString dir;

    // If there is a Forward
    if(folderForwardHistory->size() > 0)
    {
        // Get first item of the forward stack
        dir = folderForwardHistory->pop();

        // Append dir to folderHistory
        folderHistory->append(dir);

        // Set root index
        ui->treeViewExplorer->setRootIndex(model->index(dir));

    }

    // Show path in status bar
    this->setPath();
}
void MainWindow::on_buttonExport_clicked()
{
    exportFiles();
}
void MainWindow::on_buttonImport_clicked()
{
    importFiles();
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

void MainWindow::on_actionImport_triggered()
{
    importFiles();
}

void MainWindow::on_actionExport_triggered()
{
    exportFiles();
}

void MainWindow::setPath()
{
    // Show message
    ui->statusBar->showMessage(folderHistory->last());

    // At last item? Disable back button.
    ui->buttonBack->setEnabled(folderHistory->size() > 1);

    // At last forward item? Disable forward button.
    ui->buttonForward->setEnabled(folderForwardHistory->size() > 0);
}

void MainWindow::importFiles()
{
    QFileDialog qFile;
    // Allow selecting of multiple files
    qFile.setFileMode(QFileDialog::ExistingFiles);
    // Open File dialog
    qFile.exec();

    foreach (QString filePath, qFile.selectedFiles()) {
        qDebug() << filePath;

        // TODO: import file from filePath
    }
}

void MainWindow::exportFiles()
{
    QModelIndexList selectedRowsList = ui->treeViewExplorer->selectionModel()->selectedRows();
    foreach (QModelIndex index, selectedRowsList)
    {
        qDebug() << modelDirs->filePath(index);

        // TODO: Export selected file.
    }
}
