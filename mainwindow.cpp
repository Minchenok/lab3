#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) // Конструктор класса
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    data.region = allocateString(SIZE_LINE);
    data.path = allocateString(SIZE_LINE);
    data.content = NULL;
    ui->setupUi(this);
    connect(ui->pushButtonAddFile, SIGNAL(clicked()), this, SLOT(openWindowFile()));
    connect(ui->pushButtonLoadData, SIGNAL(clicked()), this, SLOT(loadData()));
    connect(ui->pushButtonCalc, SIGNAL(clicked()), this, SLOT(Calculate()));
}


MainWindow::~MainWindow() // Деструктор класса
{
    clearString(data.region);
    clearString(data.path);
    if (data.content){
        clearMatrix(data.content, data.size, data.columCount);
        clearMas(data.columName, data.columCount);
    }
    delete ui;
}


void MainWindow::showErrorWindow(char* nameError){ // Создания окна ошибок
    QMessageBox::warning(this, "ERROR", nameError);
}


void MainWindow::Calculate(){ // Обработка нажатия на "calc"
    QString colunm = ui->lineColumn->text();
    if (colunm.isEmpty() || !data.content){
        showErrorWindow("Нельзя посчитать данные");
    }
    else {
        int numCol = colunm.toInt() - 1;
        Numbers numbers;
        data.col = numCol;
        if (!doOperation(CALC, &data, &numbers)){
            ui->textMax->setText(QString::number(numbers.max));
            ui->textMin->setText(QString::number(numbers.min));
            ui->textMediana->setText(QString::number(numbers.mediana));
            buildGraph(&numbers);
        }
        else {
            showErrorWindow("Некорректный номер колонки");
        }
    }
}


void MainWindow::buildGraph(Numbers* numbers){ // Отрисовка графика

    GraphInfo graphInfo;
    graphInfo.widthWindow = ui->graph->width();
    graphInfo.heightWindow = ui->graph->height();
    graphInfo.indent = INDENT;
    QPixmap *pix = new QPixmap(graphInfo.widthWindow, graphInfo.heightWindow);
    QPainter paint(pix);
    paint.fillRect(0, 0, graphInfo.widthWindow, graphInfo.heightWindow, QBrush(QColor(255,255,255)));
    buildAxis(&paint, &graphInfo, numbers); // рисуем оси
    SendData* sendData = new SendData[data.size];
    setSendData(&data, sendData, numbers); // заполнение данных
    addDataToGraph(&paint, &graphInfo, sendData, numbers, data.size); // сама отрисовка графика
    paintValue(&paint, &graphInfo, numbers); // рисует значения по бокам
    paintMedian(&paint, &graphInfo, numbers->mediana - numbers->newMin);

    ui->graph->setPixmap(*pix);
}


void MainWindow::loadData(){ // Обработка нажатия на "загрузить данные"
    QString region = ui->lineRegion->text();
    if (region.isEmpty()){
        showErrorWindow("Не заполнено поле регион");
    }
    else {
        data.region = QstringToCharArray(region);
        Numbers numbers;
        if (!doOperation(LOAD_DATA, &data, &numbers))
            showData(&data);
        else {
            showErrorWindow("Не возможно открыть файл");
        }
    }
}


void MainWindow::openWindowFile(){ // Обработка нажатия на "Добавить файл"
    QString pathFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "CSV File (*.csv)");
    if (!pathFileName.isEmpty()){
        data.path = QstringToCharArray(pathFileName);
        ui->labelFileName->setText(pathFileName);
    }
}


char* MainWindow::QstringToCharArray(QString qstr){ // преобразование QString в char*
    char *str = (char*)malloc(sizeof(char)*(qstr.size() + 1));
    int i;
    for (i = 0; i < qstr.size(); i++){
        str[i] = qstr.at(i).unicode();
    }
    str[i] = 0;
    return str;
}


QStringList MainWindow::ConvertRowToQTFormat(char** row, size_t size){ // // преобразование char** в QStringList
    QStringList qsl = {};

    for(size_t i = 0; i < size; i++){
        qsl.append(QString::fromUtf8(row[i]));
    }

    return qsl;
}


void MainWindow::showData(Data* data){ // Добавление данных в таблицу
    ui->tableWidget->setColumnCount(data->columCount);
    QStringList QColumns = ConvertRowToQTFormat(data->columName, data->columCount);
    ui->tableWidget->setHorizontalHeaderLabels(QColumns);
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < data->size; i++){
        QStringList currentRow = ConvertRowToQTFormat(data->content[i], data->columCount);
        ui->tableWidget->setRowCount(i + 1);
        for (int j = 0; j < currentRow.count(); j++){
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::EditRole, currentRow.at(j).toDouble());
            item->setText(currentRow.at(j));
            ui->tableWidget->setItem(i, j, item);
        }
    }
}
