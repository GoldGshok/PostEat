#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnWrite,SIGNAL(pressed()),SLOT(btnWritePress()));
    connect(ui->checkBeginMaterials,SIGNAL(pressed()),SLOT(checkBeginMaterials()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTableWidget *MainWindow::createTableInventory()
{
    QTableWidget *tableInventory = new QTableWidget(2,countInventory,this);

    QStringList verticalHeader;
    verticalHeader.append("x");
    verticalHeader.append("P(x)");

    tableInventory->setVerticalHeaderLabels(verticalHeader);

    return tableInventory;
}

QTableWidget *MainWindow::createTableMaterials()
{
    QTableWidget *tableMaterials = new QTableWidget(2,countMaterials,this);

    QStringList verticalHeader;
    verticalHeader.append("y");
    verticalHeader.append("Phi(y)");

    tableMaterials->setVerticalHeaderLabels(verticalHeader);

    return tableMaterials;
}

QTableWidget *MainWindow::createTableTime()
{
    QTableWidget *tableTime = new QTableWidget(1,t,this);

    QStringList verticalHeader;
    verticalHeader.append("t");

    QStringList horizontalHeader;
    for (int i = t; i >= 1; i--){
        horizontalHeader.append('d' + QString::number(i));
    }

    tableTime->setVerticalHeaderLabels(verticalHeader);
    tableTime->setHorizontalHeaderLabels(horizontalHeader);

    return tableTime;
}

int MainWindow::f(int step)
{
    int lstep = t - step;
    int min = 0;
    while (lstep != 0){
        min += d[lstep];
        lstep--;
    }
    min = min(d[t-step],M);

    int count = min / delta;

    int **table = new int*[count + 1];
    for (int i = 0; i < count + 1; i++){
        table[i] = new int[count + 1];
    }

    for (int i = 0; i < count; i++){
        table[0][i+1] = i * delta;
        table[i+1][0] = i * delta;
    }

    for (int i = 1; i < count + 1; i++){
        for (int j = 1; j < count + 1; j++){
            if (table[i][0] + table[0][j] - d[t-step] < 0)
                table[i][j] = -1;
            else{
                table[i][j] = P(x) + phi(d[t-step]/2 +
                                         (table[i][0] + table[0][j] - d[t-step]));
                        /*+ f(table[i][0] + table[0][j] - d[step]))*/
            }
        }
    }


}

void MainWindow::btnWritePress()
{
    if (ui->edtInventory->text() != ""){
        countInventory = ui->edtInventory->text().toInt();

        x = new int[countInventory];
        Px = new int[countInventory];
    }else{
        QMessageBox msgBox;
        msgBox.setText("Поле \"количество затрат на пополнение запасов\" не может быть пустым!");
        msgBox.exec();
        msgBox.deleteLater();
    }

    if (ui->edtMaterials->text() != ""){
        countMaterials = ui->edtMaterials->text().toInt();

        y = new int[countMaterials];
        Phy = new int[countMaterials];
    }else{
        QMessageBox msgBox;
        msgBox.setText("Поле \"количество затрат на хранение сырья на складах\" не может быть пустым!");
        msgBox.exec();
        msgBox.deleteLater();
    }

    if (ui->edtT->text() != ""){
        t = ui->edtT->text().toInt();

        d = new int[t];
    }else{
        QMessageBox msgBox;
        msgBox.setText("Поле \"количество месяцев\" не может быть пустым!");
        msgBox.exec();
        msgBox.deleteLater();
    }

    if (ui->edtM->text() != ""){
        M = ui->edtM->text().toInt();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Поле \"Размер склада\" не может быть пустым!");
        msgBox.exec();
        msgBox.deleteLater();
    }

    if (ui->edtDelta->text() != ""){
        delta = ui->edtDelta->text().toInt();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Поле \"Пополнение по\" не может быть пустым!");
        msgBox.exec();
        msgBox.deleteLater();
    }

    if (ui->checkBeginMaterials->isChecked())
        if (ui->edtBeginMaterials->text() != ""){
            beginMaterials = ui->edtBeginMaterials->text().toInt();
        }else{
            QMessageBox msgBox;
            msgBox.setText("Поле \"Начальный запас\" не может быть пустым!");
            msgBox.exec();
            msgBox.deleteLater();
        }

    if (t * countMaterials * countInventory != 0){
        QDialog *d = new QDialog();

        QSizePolicy sizePolicy;

        sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
        sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);

        d->setSizePolicy(sizePolicy);

        d->setGeometry(100, 100, 800, 600);

        QVBoxLayout *l = new QVBoxLayout();
        d->setLayout(l);

        QPushButton *btn = new QPushButton();
        btn->setText("OK");

        connect(btn,SIGNAL(pressed()),SLOT(addTableValues()));
        connect(btn,SIGNAL(released()),d,SLOT(deleteLater()));

        inventory = createTableInventory();
        materials = createTableMaterials();
        time = createTableTime();

        l->addWidget(inventory);
        l->addWidget(materials);
        l->addWidget(time);
        l->addWidget(btn);

        d->exec();
    }
}

void MainWindow::checkBeginMaterials()
{
    if (!ui->checkBeginMaterials->isChecked()){
        ui->labelBeginMaterials->setEnabled(true);
        ui->edtBeginMaterials->setEnabled(true);
    }else{
        ui->labelBeginMaterials->setEnabled(false);
        ui->edtBeginMaterials->setEnabled(false);
    }
}

void MainWindow::addTableValues()
{
    for (int i = 0; i < inventory->columnCount(); i++){
        x[i] = inventory->item(0,i)->text().toInt();
        Px[i] = inventory->item(1,i)->text().toInt();
    }

    for (int i = 0; i < materials->columnCount(); i++){
        y[i] = materials->item(0,i)->text().toInt();
        Phy[i] = materials->item(1,i)->text().toInt();
    }

    for (int i = 0; i < time->columnCount(); i++){
        d[i] = time->item(0,i)->text().toInt();
    }
}
