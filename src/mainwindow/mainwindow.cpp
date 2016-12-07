#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogopen.h"
#include "dialognew.h"

#include <QDirModel>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDir>
#include <QDesktopServices>
#include <QFileSystemWatcher>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup the UI
    ui->setupUi(this);

    // Setup history stuff
    folderHistory = new QStack<QString>();
    folderForwardHistory = new QStack<QString>();

    // Create the partition safe instance
    psInstance = new PartitionSafe();

    // Setup models
    model = new PSFileSystemModel(this, psInstance);
    modelDirs = new PSFileSystemModel(this, psInstance);

    // Setup a file watcher, it detect changes of files that are currently been edited
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));

#ifdef QT_DEBUG
    // Debug mode only, load a default vault
    initializeVault("/tmp/marc.vault", "/tmp/marc.keystore");
    // Show path in status bar
    this->setPath();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
    delete folderHistory;
    delete folderForwardHistory;
    delete model;
    delete modelDirs;
}

void MainWindow::fileChanged(const QString & file){

    std::cout << "The file '" << file.toLatin1().data() << "' has been modified" << std::endl;
    QFileInfo fileInfo(file);
    psInstance->importFile(file.toLatin1().data(), fileInfo.fileName().toLatin1().data());

}

void MainWindow::on_treeViewExplorer_doubleClicked(const QModelIndex &index)
{
    // We should get the selected item
    Entry* item = model->getFile(index);

    // The item is a directory
    if(item->isDirectory()){

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

    // The double clicked item seems to be a file
    else{
        // Export the file to a temporary location
        QString tmpFile = QDir::tempPath().append("/").append(item->name.data());

        psInstance->exportFile(item->getFullPath().data(), tmpFile.toLatin1().data());

        // Watch this temp file for changes
        watcher->addPath(tmpFile);

        // Try to open it with the default application
        QDesktopServices::openUrl(QUrl(tmpFile.prepend("file://"), QUrl::TolerantMode));
    }

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
        folderForwardHistory->append(model->getCurrentDirectory());

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
        folderHistory->append(model->getCurrentDirectory());

        // Set directory
        model->setCurrentDirectory(dir);
    }

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_actionOpen_triggered()
{
    // Open the dialog
    DialogOpen *open = new DialogOpen(this);
    int dialogResult = open->exec();

    // Accepted dialog?
    if(dialogResult == QDialog::Accepted) {
        // Open the vault
        initializeVault(open->locationVault, open->locationKeyStore);
    }
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
    qFile.setFileMode(QFileDialog::ExistingFiles);
    // Open File dialog
    qFile.exec();

    int filesSelected =qFile.selectedFiles().count();
    int current = 0;

    QProgressDialog dialog(this);
    dialog.setLabelText(tr("Importing file..."));
    dialog.setRange(0, filesSelected);
    dialog.setVisible(true);
    dialog.setModal(true);

    // Import files one by one
    foreach (QString filePath, qFile.selectedFiles()) {
        qDebug() << filePath;
        QFileInfo fileInfo(filePath);

        dialog.setLabelText(tr("Importing file: %1").arg(fileInfo.fileName()));
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

        QString destinationPath  = model->getCurrentDirectory().append("/").append(fileInfo.fileName());
        qDebug() << destinationPath;
        psInstance->importFile(filePath.toLatin1().data(),destinationPath.toLatin1().data());

        dialog.setValue(++current);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    model->setCurrentDirectory(model->getCurrentDirectory());
    model->layoutChanged();
}

void MainWindow::importFolder(){
        qDebug() << "importFolder() called";

        QFileDialog qFile;
        // Allow selecting of multiple files
        qFile.setFileMode(QFileDialog::Directory);
        // Open File dialog
        qFile.exec();

        foreach (QString folderPath, qFile.selectedFiles()) {
            qDebug() << folderPath;

            // TODO: import folder from filePath
        }
}

void MainWindow::exportFiles()
{
    QFileDialog qFile;
    // Allow selecting of multiple files
    qFile.setFileMode(QFileDialog::DirectoryOnly);

    // Get the destination directory from the user
    QString destinationDir = qFile.getExistingDirectory();

    QModelIndexList selectedRowsList = ui->treeViewExplorer->selectionModel()->selectedRows();
    foreach (QModelIndex index, selectedRowsList)
    {
        QFileInfo fileInfo(model->getFile(index)->getFullPath().data());

        // Get the source and destination paths
        QString sourcePath = model->getFile(index)->getFullPath().data();
        QString destinationPath = destinationDir + "/" + fileInfo.fileName();

        qDebug() << sourcePath;
        qDebug() << destinationDir;
        qDebug() << destinationPath;

        // export the current file
        psInstance->exportFile(sourcePath.toLatin1().data(), destinationPath.toLatin1().data());
    }
}

void MainWindow::initializeVault(const std::string vaultPath, const std::string keyStorePath)
{
    try {
        // Convert names
        const char *cVaultPath = vaultPath.c_str();
        const char *cKeyStorePath = keyStorePath.c_str();

        // Setup vault
        psInstance->init(cVaultPath, cKeyStorePath);
        psInstance->open();

        // Create file system models and other instances
        folderHistory->clear();
        folderForwardHistory->clear();

        // Initialize models
        model->init();
        modelDirs->init();

        // Set models in views
        ui->treeViewExplorer->setModel(model);
        ui->treeViewFiles->setModel(modelDirs);

        // Enable import/export
        ui->buttonImport->setEnabled(true);
        ui->buttonExport->setEnabled(true);

        // Set paths
        this->setPath();
    } catch(const char *exception) {
        std::cout << "Exception: " << exception << std::endl;
    }
}

void MainWindow::setPath()
{
    // Show message
    if(model != nullptr) ui->statusBar->showMessage(model->getCurrentDirectory());

    // At last item? Disable back button.
    ui->buttonBack->setEnabled(folderHistory->size() > 0);

    // At last forward item? Disable forward button.
    ui->buttonForward->setEnabled(folderForwardHistory->size() > 0);
}
