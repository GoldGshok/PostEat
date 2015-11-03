#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QLayout>
#include <QSizePolicy>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTableWidget *inventory;
    QTableWidget *materials;
    QTableWidget *time;

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

    QTableWidget *createTableInventory();
    QTableWidget *createTableMaterials();
    QTableWidget *createTableTime();

    int *f(int step);
    int *fn(int step);
    int P(int x);
    int Phi(int y);
    int min(int d, int m){ return !(d < m) ? d : m; }

private slots:
    void btnWritePress();
    void checkBeginMaterials();
    void addTableValues();
};

#endif // MAINWINDOW_H
