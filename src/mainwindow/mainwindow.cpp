#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    folderHistory = new QStack<QString>;

    ui->setupUi(this);

    model = new QFileSystemModel;
    model->setRootPath(QDir::homePath());

    // Add to stack
    folderHistory->append(QDir::homePath());

    // Show path in status bar
    this->setPath();

    ui->treeView_2->setModel(model);
    ui->treeView_2->setRootIndex(model->index(QDir::homePath()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_2_doubleClicked(const QModelIndex &index)
{
    // Get directory
    QString dir(model->filePath(index));

    // Add to stack
    folderHistory->append(dir);

    // Set root index
    ui->treeView_2->setRootIndex(model->index(dir));

    // Show path in status bar
    this->setPath();
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
        if(folderHistory->size() > 0 && ui->treeView_2->rootIndex() == model->index(dir)) {
            // Pop another
            dir = folderHistory->last();
        }

        // Set root index
        ui->treeView_2->setRootIndex(model->index(dir));
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


