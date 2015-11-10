#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <QLayout>
#include <QSizePolicy>
#include <vector>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace Ui {
class MainWindow;
}

struct memory{
    std::vector<int> xt;
    std::vector<int> ft;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTableWidget *inventory;
    QTableWidget *materials;
    QTableWidget *time;

    void htmlFn(int **mas, std::vector<int> f, std::vector<int> x, int count);
    void htmlF(int **mas, std::vector<int> f, std::vector<int> x, int count, int step);
    void htmlResult(int **mas, int fmin);

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

    std::vector<memory> Output; //для хранения всех f(x), x(y)

    QTableWidget *createTableInventory();
    QTableWidget *createTableMaterials();
    QTableWidget *createTableTime();

    std::vector<int> f(int step);
    std::vector<int> fn();
    int P(int x);
    int Phi(int y);
    int min(int a, int b){ return (a < b) ? a : b; }

private slots:
    void btnWritePress();
    void checkBeginMaterials();
    void addTableValues();
    void reshenie();
    void readJSON();
};

#endif // MAINWINDOW_H
