#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    folderHistory(new QStack<QString>)
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
    delete model;
    delete modelDirs;
}

void MainWindow::on_treeViewExplorer_doubleClicked(const QModelIndex &index)
{
    // Get directory
    QString dir(model->filePath(index));

    // Add to stack
    folderHistory->append(dir);

    // Set root index
    ui->treeViewExplorer->setRootIndex(model->index(dir));

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_treeViewFiles_clicked(const QModelIndex &index) {
    this->on_treeViewExplorer_doubleClicked(index);
}

void MainWindow::on_buttonBack_clicked()
{
    // Track dir
    QString dir;

    // More than only the home item
    if(folderHistory->size() > 0) {
        // Get the first item of the stack
        dir = folderHistory->pop();

        // Is it the current directory?
        if(folderHistory->size() > 0 && ui->treeViewExplorer->rootIndex() == model->index(dir)) {
            // Pop another
            dir = folderHistory->last();
        }

        // Set root index
        ui->treeViewExplorer->setRootIndex(model->index(dir));
    }

    // History empty?
    if(folderHistory->size() == 0) {
        // Append the last dir as it must be the starting point
        folderHistory->append(dir);
    }

    // Show path in status bar
    this->setPath();
}

void MainWindow::setPath()
{
    // Show message
    ui->statusBar->showMessage(folderHistory->last());

    // At last item? Disable back button.
    ui->buttonBack->setEnabled(folderHistory->size() > 1);
}
