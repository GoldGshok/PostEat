#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //соединяем действия и события
    connect(ui->btnWrite,SIGNAL(pressed()),SLOT(btnWritePress()));
    connect(ui->checkBeginMaterials,SIGNAL(pressed()),SLOT(checkBeginMaterials()));
    connect(ui->readFile,SIGNAL(triggered(bool)),SLOT(readJSON()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::htmlFn(int **table, std::vector<int> f, std::vector<int> x, int count)
{
    //записываем вывод первой таблицы в наглядной форме
    QFile file("output.html");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){

    }else{
        QTextStream out(&file);
        out << QString("<!DOCTYPE html>").toUtf8() << QString("\n").toUtf8();
        out << QString("<html>"
               "<head>"
                 "<meta charset=\"windows-1251\">"
                 "<title>Решение</title>"
               "</head>"
               "<body>").toUtf8();

        out << QString("<table border=1 align=center>").toUtf8()
            << QString("\n").toUtf8();
        out << QString("<tr>"
                    "<th>y</th>"
                    "<th>x(y)</th>"
                    "<th>f_1(y)</th>"
               "</tr>").toUtf8() << QString("\n").toUtf8();
        for (int i = 0; i < count; i++){
            out << QString("<tr>").toUtf8() << QString("\n").toUtf8();

            out << QString("<td>").toUtf8() << table[i][0]
                << QString("</td>").toUtf8();

            out << QString("<td>").toUtf8() << x[i] << QString("</td>").toUtf8();
            out << QString("<td>").toUtf8() << f[i] << QString("</td>").toUtf8();
            out << QString("</tr>").toUtf8() << QString("\n").toUtf8();
        }
        out << QString("</table>").toUtf8() << QString("\n").toUtf8();
    }
    file.close();
}

void MainWindow::htmlF(int **table, std::vector<int> f, std::vector<int> x, int count, int step)
{
    //записываем вывод остальных таблиц в наглядной форме
    QFile file("output.html");
    if (!file.open(QIODevice::Text | QIODevice::Append)){

    }else{
        QTextStream out(&file);

        out << QString("<table border=1 align=center>").toUtf8()
            << QString("\n").toUtf8();
        out << QString("<caption>Таблица результатов</caption>").toUtf8();
        out << QString("<tr>"
                    "<th>y/x</th>").toUtf8();
        for (int i = 1; i < count; i++){
            out << QString("<th>").toUtf8() << table[0][i] << QString("</th>").toUtf8();
        }

        out <<     QString("<th>x(y)</th>"
                    "<th>f_").toUtf8() << step << QString("(y)</th>"
               "</tr>").toUtf8() << QString("\n").toUtf8();
        for (int i = 1; i < count; i++){
            out << QString("<tr>").toUtf8() << QString("\n").toUtf8();
            for (int j = 0; j < count; j++){
                out << QString("<td>").toUtf8() << table[i][j]
                       << QString("</td>").toUtf8();
            }
            out << QString("<td>").toUtf8() << x[i-1] << QString("</td>").toUtf8();
            out << QString("<td>").toUtf8() << f[i-1] << QString("</td>").toUtf8();
            out << QString("</tr>").toUtf8() << QString("\n").toUtf8();
        }
        out << QString("</table>").toUtf8() << QString("\n").toUtf8();
    }
    file.close();
}

void MainWindow::htmlResult(int **mas, int fmin)
{
    //вывод итоговой таблицы
    QFile file("output.html");
    if (!file.open(QIODevice::Text | QIODevice::Append)){

    }else{
        QTextStream out(&file);

        out << QString("<table border=1 align=center>").toUtf8()
            << QString("\n").toUtf8();
        out << QString("<caption>Ответ</caption>").toUtf8();
        out << QString("<tr>"
                    "<th>dn</th>").toUtf8();
        for (int i = 0; i < t; i++){
            out << QString("<th>").toUtf8() << d[i] << QString("</th>").toUtf8();
        }
        out << QString("</tr>").toUtf8() << QString("\n").toUtf8();

        for (int i = 0; i < 4; i++){
            out << QString("<tr>").toUtf8() << QString("\n").toUtf8();

            out << QString("<td>").toUtf8() << i << QString("</td>").toUtf8();
            for (int j = 0; j < t; j++){
                out << QString("<td>").toUtf8() << mas[i][j]
                       << QString("</td>").toUtf8();
            }
            out << QString("</tr>").toUtf8() << QString("\n").toUtf8();
        }
        out << QString("</table>").toUtf8() << QString("\n").toUtf8();

        //here fmin and x[i],y[i]
        out << QString("<p>").toUtf8() << QString("\n").toUtf8();
        out << QString("Fmin = ").toUtf8() << fmin << QString("; ").toUtf8();
        out << QString("</p>").toUtf8() << QString("\n").toUtf8();

        out << QString("<p>").toUtf8() << QString("\n").toUtf8();
        for (int i = 0; i < t; i++){
            out << QString("x").toUtf8() << i+1 << QString(" = ").toUtf8() << mas[0][i] << QString("; ").toUtf8();
        }
        out << QString("</p>").toUtf8() << QString("\n").toUtf8();

        out << QString("<p>").toUtf8() << QString("\n").toUtf8();
        for (int i = 0; i < t; i++){
            out << QString("y").toUtf8() << i+1 << QString(" = ").toUtf8() << mas[1][i] << QString("; ").toUtf8();
        }
        out << QString("</p>").toUtf8() << QString("\n").toUtf8();

        //close html
        out << QString("</body>").toUtf8();
        out << QString("</html>").toUtf8();
    }
    file.close();
}

QTableWidget *MainWindow::createTableInventory()
{
    //создание таблицы для ввода инвентаря
    QTableWidget *tableInventory = new QTableWidget(2,countInventory,this);

    QStringList verticalHeader;
    verticalHeader.append("x");
    verticalHeader.append("P(x)");

    tableInventory->setVerticalHeaderLabels(verticalHeader);

    return tableInventory;
}

QTableWidget *MainWindow::createTableMaterials()
{
    //создание таблицы для ввода материалов
    QTableWidget *tableMaterials = new QTableWidget(2,countMaterials,this);

    QStringList verticalHeader;
    verticalHeader.append("y");
    verticalHeader.append("Phi(y)");

    tableMaterials->setVerticalHeaderLabels(verticalHeader);

    return tableMaterials;
}

QTableWidget *MainWindow::createTableTime()
{
    //создание таблицы для ввода месяцев
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
    //расчет всех таблиц кроме первой
    int lstep = t - step;
    int minf = 0;
    for (int i = lstep; i < t; i++){
        minf += d[i];
    }
    minf = min(minf,M);

    int count = minf / delta + 2;

    //std::vector<int> func;
    //std::vector<int> x_y;
    memory memoryTemp;

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

    for (int i = 1; i < count; i++){
        int minJ = table[i][1];
        int tempX = table[0][1];
        for (int j = 1; j < count; j++){
            if (minJ > table[i][j]){
                minJ = table[i][j];
                tempX = table[0][j];
            }
        }
        memoryTemp.xt.push_back(tempX);
        memoryTemp.ft.push_back(minJ);
    }

    Output.push_back(memoryTemp);
    htmlF(table,memoryTemp.ft,memoryTemp.xt,count,lstep);

    return memoryTemp.ft;
}

std::vector<int> MainWindow::fn()
{
    //расчет первой таблицы
    int lstep = t - 1;

    int minf = min(d[lstep],M);

    int count = minf / delta;

    //std::vector<int> func;
    memory memoryTemp;

    int **table = new int*[count+1];
    for (int i = 0; i < count+1; i++){
        table[i] = new int[count+1];
    }

    for (int i = 0; i < count+1; i++){
        memoryTemp.xt.push_back(d[lstep] - i * delta);
        table[i][0] = i * delta;
    }

    for (int i = 0; i < count+1; i++){
        memoryTemp.ft.push_back(P(memoryTemp.xt[i]) + Phi(d[lstep]/2));
    }

    Output.push_back(memoryTemp);

    htmlFn(table,memoryTemp.ft,memoryTemp.xt,count+1);

    return memoryTemp.ft;
}

int MainWindow::P(int x)
{
    //поиск нужного P[x]
    for (int i = 0; i < countInventory; i++){
        if (this->x[i] == x)
        {
            return Px[i];
        }
    }
}

int MainWindow::Phi(int y)
{
    //поиск нужного Phi[y]
    for (int i = 0; i < countMaterials; i++){
        if (this->y[i] == y)
        {
            return Phy[i];
        }
    }
}

void MainWindow::btnWritePress()
{
    //считываем значения констант
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
        //создаем форму для заполения вспомогательных таблиц
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
    //если активированы начальные запасы сменить состояние кнопки и поля
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
    //заполнение значений из таблиц
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
    //решаем задачу
    //std::vector<int> rez;
    f(t);

    int **table = new int*[t];
    for (int i = 0; i < 4; i++){
        table[i] = new int[t];
    }

    int index = 0;
    for (int i = 1; i < Output[Output.size()-1].ft.size(); i++){
        if (Output[Output.size()-1].ft[index] > Output[Output.size()-1].ft[i]){
            index = i;
        }
    }

    table[0][0] = Output[Output.size()-1].xt[index];
    table[1][0] = index * delta;
    table[2][0] = table[0][0] + table[1][0];
    table[3][0] = table[2][0] - d[0];
    for (int i = 1; i < t; i++){
        table[0][i] = Output[Output.size()-1-i].xt[table[3][i-1] / delta];
        table[1][i] = table[3][i-1];
        table[2][i] = table[0][i] + table[1][i];
        table[3][i] = table[2][i] - d[i];
    }

    htmlResult(table, Output[Output.size()-1].ft[index]);
}

void MainWindow::readJSON()
{
    //читаем input.json
    QString settings;
    QFile file;
    file.setFileName("input.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        settings = file.readAll();
    }else{
        QMessageBox msgBox;
        msgBox.setText("File \"input.json\" not found!");
        msgBox.exec();
    }
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(settings.toUtf8());
    if (!document.isNull()){
        QJsonObject object = document.object();

        countInventory = object["Inventory"].toString().toInt();
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

        QMessageBox msgBox;
        QPushButton *ok = msgBox.addButton(tr("OK"),QMessageBox::ActionRole);
        connect(ok,SIGNAL(pressed()),SLOT(reshenie()));
        msgBox.setText("Reading complete!");
        msgBox.exec();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Reading failed!");
        msgBox.exec();
    }
}
