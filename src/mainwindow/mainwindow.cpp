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
    modelDirs = new PSTreeFileSystemModel(this, psInstance);
    modelDirs->setDirectoriesOnly(true);

    // Setup a file watcher, it detect changes of files that are currently been edited
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));

#ifdef QT_DEBUG
#ifndef __WIN32
        const char *vaultPath = "/tmp/marc.vault";
        const char *keyStorePath = "/tmp/marc.keystore";
#else
        wchar_t _vaultPath[1024];
        wchar_t _keyStorePath[1024];
        ExpandEnvironmentStrings(L"%Temp%\\marc.vault", _vaultPath, 1024);
        ExpandEnvironmentStrings(L"%Temp%\\marc.keystore", _keyStorePath, 1024);
        char vaultPath[1024];
        char keyStorePath[1024];
        wcstombs(vaultPath, _vaultPath, 1024);
        wcstombs(keyStorePath, _keyStorePath, 1024);
#endif

    // Debug mode only, load a default vault
    initializeVault((const char *)vaultPath, (const char *)keyStorePath, "test", "test");

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
    delete psInstance;
}

void MainWindow::fileChanged(const QString & file){

    std::cout << "The file '" << file.toLatin1().data() << "' has been modified" << std::endl;
    QFileInfo fileInfo(file);
    psInstance->getVault()->getPartition()->importFile(file.toLatin1().data(), fileInfo.fileName().toLatin1().data());

}

void MainWindow::on_treeViewExplorer_doubleClicked(const QModelIndex &index)
{
    // We should get the selected item
    Entry* item = model->getFile(index);

    // The item is a directory
    if(item->isDirectory()){

        // Get the path
        QString path = QString(item->getFullPath().c_str());

        // Enter given directory and add to history
        model->enterDirectory(path, *folderHistory, *folderForwardHistory);

        // Show path in status bar
        this->setPath();

    }

    // The double clicked item seems to be a file
    else{
        // Export the file to a temporary location
        QString tmpFile = QDir::tempPath().append("/").append(item->name.data());

        psInstance->getVault()->getPartition()->exportFile(item->getFullPath().data(), tmpFile.toLatin1().data());

        // Watch this temp file for changes
        watcher->addPath(tmpFile);

        // Try to open it with the default application
        QDesktopServices::openUrl(QUrl(tmpFile.prepend("file://"), QUrl::TolerantMode));
    }

}

void MainWindow::on_buttonBack_clicked()
{
    model->navigation_buttons(*folderHistory, *folderForwardHistory);

    // Show path in status bar
    this->setPath();
}

void MainWindow::on_buttonForward_clicked()
{
    model->navigation_buttons(*folderForwardHistory, *folderHistory);

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
        initializeVault(open->locationVault, open->locationKeyStore, open->username, open->password);
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

void MainWindow::on_buttonDelete_clicked()
{
    deleteFileDirectory();
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
        model->importFile(filePath.toLatin1().data(),destinationPath.toLatin1().data());
        //psInstance->importFile(filePath.toLatin1().data(),destinationPath.toLatin1().data());
        dialog.setValue(++current);
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MainWindow::importFolder()
{
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
        psInstance->getVault()->getPartition()->exportFile(sourcePath.toLatin1().data(), destinationPath.toLatin1().data());
    }
}

void MainWindow::deleteFileDirectory()
{
    model->deleteFileDirectory(selectedRowsList);
    on_treeViewExplorer_selectionChanged();
}

void MainWindow::initializeVault(const std::string vaultPath, const std::string keyStorePath, const std::string username, const std::string password)
{
    try {
        // Setup vault
        psInstance->init(vaultPath.c_str(), keyStorePath.c_str(), username.c_str(), password.c_str());
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

        // TreeViewExplorer: Add signal slot for detecting selection.
        connect(ui->treeViewExplorer->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(on_treeViewExplorer_selectionChanged()));

        // Enable import/export/Delete
        ui->buttonImport->setEnabled(true);
        ui->actionImports->setEnabled(true);
        ui->actionFile->setEnabled(true);

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
    if(modelDirs != nullptr) modelDirs->init();

    // At last item? Disable back button.
    ui->buttonBack->setEnabled(folderHistory->size() > 0);

    // At last forward item? Disable forward button.
    ui->buttonForward->setEnabled(folderForwardHistory->size() > 0);
}

void MainWindow::on_treeViewExplorer_selectionChanged()
{
    //checks if someting is selected
    selectedRowsList = ui->treeViewExplorer->selectionModel()->selectedRows();
    bool hasSelection = selectedRowsList.size()>=1;

    // Enable export/delete
    ui->buttonDelete->setEnabled(hasSelection);
    ui->buttonExport->setEnabled(hasSelection);
    ui->actionExport->setEnabled(hasSelection);
}
