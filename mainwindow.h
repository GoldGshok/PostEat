#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int countInventory = 0;
    int countMaterials = 0;
    int t = 0;

    int *x;
    int *Px;

    int *y;
    int *Phy;

    int *d;

    int M = 0;
    int delta = 0;
    int beginMaterials = 0;

    void createTableInventory();
    void createTableMaterials();
    void createTableTime();

private slots:
    void btnWritePress();
    void checkBeginMaterials();
};

#endif // MAINWINDOW_H
