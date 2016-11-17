#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileSystemModel>
#include <QMainWindow>
#include <QStack>
#include <QString>

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
    void on_actionOpen_triggered();
    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QFileSystemModel *modelDirs;
    QStack<QString> *folderHistory, *folderForwardHistory;

    void setPath();
};

#endif // MAINWINDOW_H
