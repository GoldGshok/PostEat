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

void MainWindow::createTableInventory()
{
    QTableWidget *tableInventory = new QTableWidget(2,countInventory,this);

    QStringList verticalHeader;
    verticalHeader.append("x");
    verticalHeader.append("P(x)");

    tableInventory->setVerticalHeaderLabels(verticalHeader);


    //how it display in position
    ui->verticalLayout->addWidget(tableInventory);

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
        createTableInventory();
        //createTableMaterials();
        //createTableTime();
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
