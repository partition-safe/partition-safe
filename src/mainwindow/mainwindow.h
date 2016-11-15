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
    void on_treeView_2_doubleClicked(const QModelIndex &index);
    void on_buttonBack_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QStack<QString> *folderHistory;

    void setPath();
};

#endif // MAINWINDOW_H
