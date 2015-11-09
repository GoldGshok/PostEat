#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnWrite,SIGNAL(pressed()),SLOT(btnWritePress()));
    connect(ui->checkBeginMaterials,SIGNAL(pressed()),SLOT(checkBeginMaterials()));
    connect(ui->readFile,SIGNAL(triggered(bool)),SLOT(readJSON()));
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

std::vector<int> MainWindow::f(int step)
{
    int lstep = t - step;
    int minf = 0;
    for (int i = lstep; i < t; i++){
        minf += d[i];
    }
    minf = min(minf,M);

    int count = minf / delta + 2;

    std::vector<int> func;
    std::vector<int> fb;
    if (step == 2){
        fb = this->fn();
    }else{
        fb = f(step-1);
    }

    int **table = new int*[count+1];
    for (int i = 0; i < count+1; i++){
        table[i] = new int[count+1];
    }

    for (int i = 0; i < count; i++){
        table[0][i+1] = i * delta;
        table[i+1][0] = i * delta;
    }

    for (int i = 1; i < count; i++){
        for (int j = 1; j < count; j++){
            if (table[j][0] + table[0][i] - d[lstep] < 0)
                table[j][i] = 9999999;
            else
            if (table[j][0] + table[0][i] > minf){
                table[j][i] = 9999999;
            }else{
                table[j][i] = P(table[0][i]) +
                        Phi(d[lstep]/2 + (table[j][0] + table[0][i] - d[lstep])) +
                        fb[(table[j][0] + table[0][i] - d[lstep]) / delta];
            }

        }
    }

    /*for (int i = 0; i < count; i++){
        for (int j = 0; j < count; j++){
            qDebug() << table[i][j] << " ";
        }
    }*/

    for (int i = 1; i < count; i++){
        int minJ = table[i][1];
        for (int j = 1; j < count; j++){
            minJ = min(minJ,table[i][j]);
        }
        func.push_back(minJ);
    }

    return func;
}

std::vector<int> MainWindow::fn()
{
    int lstep = t - 1;

    int minf = min(d[lstep],M);

    int count = minf / delta;

    std::vector<int> func;

    int **table = new int*[count+1];
    for (int i = 0; i < count+1; i++){
        table[i] = new int[count+1];
    }

    for (int i = 0; i < count+1; i++){
        table[i][1] = d[lstep] - i * delta;
        table[i][0] = i * delta;
    }

    for (int i = 0; i < count+1; i++){
        func.push_back(P(table[i][1]) + Phi(d[lstep]/2));
    }

    return func;
}

int MainWindow::P(int x)
{
    for (int i = 0; i < countInventory; i++){
        if (this->x[i] == x)
        {
            return Px[i];
        }
    }
}

int MainWindow::Phi(int y)
{
    for (int i = 0; i < countMaterials; i++){
        if (this->y[i] == y)
        {
            return Phy[i];
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
        connect(btn,SIGNAL(released()),SLOT(reshenie()));

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

void MainWindow::reshenie()
{
    std::vector<int> rez;
    rez = f(t);
    for (int i = 0; i < rez.size(); i++){
        qDebug() << rez[i] << " ";
    }
}

void MainWindow::readJSON()
{
    QString settings;
    QFile file;
    file.setFileName("input.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        settings = file.readAll();
    }
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(settings.toUtf8());
    qDebug() << document.isNull();
    QJsonObject object = document.object();

    countInventory = object["Inventory"].toString().toInt();
    qWarning() << countInventory;
    countMaterials = object["Materials"].toString().toInt();
    t = object["Month"].toString().toInt();
    delta = object["Delta"].toString().toInt();
    M = object["SizeM"].toString().toInt();
    beginMaterials = object["BMaterials"].toString().toInt();

    //таблица инвентаря
    x = new int[countInventory];
    Px = new int[countInventory];

    //таблица материалов
    y = new int[countMaterials];
    Phy = new int[countMaterials];

    //месяца
    d = new int[t];

    QJsonArray X = object.value(QString("X")).toArray();
    QJsonArray PX = object.value(QString("Px")).toArray();
    QJsonArray Y = object.value(QString("Y")).toArray();
    QJsonArray PHY = object.value(QString("Phy")).toArray();
    QJsonArray D = object.value(QString("D")).toArray();

    for (int i = 0; i < countInventory; i++){
        x[i] = X[i].toString().toInt();
        Px[i] = PX[i].toString().toInt();
    }

    for (int i = 0; i < countMaterials; i++){
        y[i] = Y[i].toString().toInt();
        Phy[i] = PHY[i].toString().toInt();
    }

    for (int i = 0; i < t; i++){
        d[i] = D[i].toString().toInt();
    }
}
