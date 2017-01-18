#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogopen.h"
#include "dialognew.h"
#include "dialognewuser.h"
#include "dialognotifications.h"
#include "dialognewdirectory.h"
#include "dialogshare.h"
#include "dialogrename.h"

#include <QDirModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QProgressDialog>
#include <QDir>
#include <QDesktopServices>
#include <QFileSystemWatcher>
#include <QUuid>
#include <NotificationCentre.h>
#include <QMessageBox>
#include <Common.h>

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
//    initializeVault((const char *)vaultPath, (const char *)keyStorePath, "youri", "12345678");

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

void MainWindow::fileChanged(const QString & file)
{
    std::cout << "The file '" << file.toLatin1().data() << "' has been modified" << std::endl;
    Entry* item = modifiedFileList.value(file);

    // If item is not null update the changed file
    if(item != NULL)
    {
        QFileInfo fileInfo(file);
        psInstance->getVault()->getPartition()->importFile(file.toLatin1().data(), item->getFullPath().c_str());
    }
}

void MainWindow::on_treeViewExplorer_doubleClicked(const QModelIndex &index)
{
    // We should get the selected item
    Entry* item = model->getFile(index);

    // The item is a directory
    if(item->isDirectory())
    {
        // Get the path
        QString path = QString(item->getFullPath().c_str());

        // Enter given directory and add to history
        model->enterDirectory(path, *folderHistory, *folderForwardHistory);

        // Show path in status bar
        this->setPath();
    }

    // The double clicked item seems to be a file
    else
    {
        QUuid uuid = QUuid::createUuid();

        // Export the file to a temporary location
        QString tmpFile = QDir::tempPath().append("/").append(uuid.toString()).append("_").append(item->name.data());

        psInstance->getVault()->getPartition()->exportFile(item->getFullPath().data(), tmpFile.toLatin1().data());

        // Watch this temp file for changes
        watcher->addPath(tmpFile);
        modifiedFileList.insert(tmpFile, item);

        // Try to open it with the default application
        QDesktopServices::openUrl(QUrl(tmpFile.prepend("file:///"), QUrl::TolerantMode));
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
    // Create dialog open
    DialogOpen *open = new DialogOpen(this);

    // Open the dialog and wait for a result.
    // On result open partition.
    if(open->exec()) initializeVault(open->locationVault, open->locationKeyStore, open->username, open->password);
}

void MainWindow::on_buttonEdit_clicked()
{
    renameFileFolder();
}

void MainWindow::on_actionRename_triggered()
{
    renameFileFolder();
}

void MainWindow::on_actionNew_triggered()
{
    // Opens dialog for a new PartitionSafe
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

void MainWindow::on_actionDelete_triggered()
{
    deleteFileDirectory();
}

void MainWindow::on_actionNew_Directory_triggered()
{
    newDirectory();
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

void MainWindow::on_buttonNewDirectory_clicked()
{
    newDirectory();
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

    if(qFile.exec()){
        foreach (QString filePath, qFile.selectedFiles()) {
            qDebug() << filePath;
            model->importFolder(filePath.toLatin1().data(), model->getCurrentDirectory().toLatin1().data());
        }
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
        QString destinationPath = destinationDir + "/" + fileInfo.fileName();

        // Get the source and destination paths
        QString sourcePath = model->getFile(index)->getFullPath().data();

        qDebug() << sourcePath;
        qDebug() << destinationDir;
        qDebug() << destinationPath;

        // export the current file
        exportFile(sourcePath, destinationPath);
    }
}

void MainWindow::exportFile(QString sourcePath, QString destinationPath)
{
    FRESULT exists;
    FILINFO fno;
    std::vector<Entry*>* subItemsListing;

    Entry* entry;

    // Check if the file or directory exists and save file info in 'fno'.
    exists = f_stat(Common::stdStringToTChar(sourcePath.toStdString()), &fno);
    if(exists == FR_OK) // If file/directory exists
    {
        switch(fno.fattrib)
        {
        case AM_DIR: // If the item is an directory
            // Make the directory on the system
            makeDir(destinationPath);

            // Get all sub-items from the selected directory (in the partition)
            subItemsListing = (std::vector<Entry*>*) psInstance->getVault()->getPartition()->listDirectory(sourcePath.toStdString());
            // for all the sub-directories
            while(subItemsListing->size()>=1)
            {
                // Get and pop a sub-item
                entry = subItemsListing->back();
                subItemsListing->pop_back();

                // Full destination path for the file to export or directory to create.
                QString fullDesPath = destinationPath + "/" + entry->name.data();
                // Full source path to the file or directory to export
                QString fullSourPath = sourcePath + "/" + entry->name.data();
                if(entry->isDirectory()) // If sub item is a directory
                {
                    // Make sub-directory on the system
                    // makeDir(fullDesPath);

                    // Recursive call for sub items of the sub-directory
                    exportFile(entry->getFullPath().c_str(), fullDesPath);
                }
                // if a sub-file, export the file.
                else psInstance->getVault()->getPartition()->exportFile(fullSourPath.toLatin1().data(), fullDesPath.toLatin1().data());
            }
            break;
        default:
            psInstance->getVault()->getPartition()->exportFile(sourcePath.toLatin1().data(), destinationPath.toLatin1().data());
            break;
        }
    }
}

void MainWindow::renameFileFolder()
{
    qDebug()<<"rename";
    QFileInfo fileInfo(model->getFile(selectedRowsList[0])->getFullPath().data());
    QString oldPath = fileInfo.fileName();
    QString newPath = "";

    DialogRename *dialogRename = new DialogRename(this, oldPath);

    int dialogResult = dialogRename->exec();

    if(dialogResult == QDialog::Accepted)
    {
        // Get new directory name
        newPath = dialogRename->fileName;
        newPath = newPath.trimmed();

        model->renameFileFolder(oldPath, newPath);
    }
}

void MainWindow::deleteFileDirectory()
{
    model->deleteFileDirectory(selectedRowsList);
    on_treeViewExplorer_selectionChanged();
}

void MainWindow::setNotifications()
{
    // Get the notifications
    auto *notifications = psInstance->getNotificationCentre()->loadNotificationsForUser(psInstance->getUser()->id);

    if (notifications->size()>0)
    {
        ui->buttonNotifications->setVisible(true);

        // Set notifications
        std::string s = "[";
        s += std::to_string(notifications->size());
        s += "]";
        ui->buttonNotifications->setText(QString(s.c_str()));
    } else
    {
        ui->buttonNotifications->setVisible(false);
    }

}

void MainWindow::makeDir(QString path)
{
    QDir().mkdir(path);
}

void MainWindow::initializeVault(const std::string vaultPath, const std::string keyStorePath, const std::string username, const std::string password)
{
    try
    {
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
        ui->buttonNewDirectory->setEnabled(true);
        ui->actionNew_Directory->setEnabled(true);
        ui->actionFile->setEnabled(true);
        ui->buttonNotifications->setEnabled(true);
        ui->actionFolder->setEnabled(true);

        // Set notifications
        setNotifications();

        // Set paths
        this->setPath();
    } catch(const char *exception)
    {
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

void MainWindow::newDirectory()
{
    DialogNewDirectory *newDialogDirectory = new DialogNewDirectory(this);

    int dialogResult = newDialogDirectory->exec();

    if(dialogResult == QDialog::Accepted)
    {
        // Get new directory name
        QString dirName = newDialogDirectory->dirName;
        dirName = dirName.trimmed();
        QString fullPath = model->getCurrentDirectory()+"/"+dirName;
        // if not existing, create new directory
        if(!model->directoryExists(fullPath)) model->createDirectory(fullPath);
        else QMessageBox::warning(0,"Can't ctory", "Directory already exists");
    }
}

void MainWindow::on_treeViewExplorer_selectionChanged()
{
    //checks if someting is selected
    selectedRowsList = ui->treeViewExplorer->selectionModel()->selectedRows();
    bool hasSelection = selectedRowsList.size()>=1;

    // Enable export/delete
    ui->buttonShare->setEnabled(selectedRowsList.size() == 1);
    ui->buttonDelete->setEnabled(hasSelection);
    ui->buttonExport->setEnabled(hasSelection);
    ui->buttonEdit->setEnabled(hasSelection);
    // Action menu
    ui->actionDelete->setEnabled(hasSelection);
    ui->actionExport->setEnabled(hasSelection);
    ui->actionRename->setEnabled(hasSelection);
}

void MainWindow::on_buttonNotifications_clicked()
{
    // Open the dialog
    DialogNotifications *open = new DialogNotifications(psInstance, this);
    open->exec();

    // Reload notifications
    setNotifications();
}

void MainWindow::on_actionUser_triggered()
{
    DialogNewUser* dialog = new DialogNewUser(this, psInstance);
    dialog->show();
}

void MainWindow::on_buttonShare_clicked()
{
    // Get the path
    QFileInfo fileInfo(model->getFile(selectedRowsList[0])->getFullPath().data());
    std::string oldPath = "/" + std::string(fileInfo.fileName().toUtf8().constData());

    // Current directory not root? Add the path.
    if(strcmp(model->getCurrentDirectory().toUtf8().constData(), "/") != 0) {
        oldPath = model->getCurrentDirectory().toUtf8().constData() + oldPath;
    }

    // Open the dialog
    DialogShare *share = new DialogShare(oldPath, this->psInstance, this);
    share->show();
}
