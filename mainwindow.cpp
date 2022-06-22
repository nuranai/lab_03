#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iocController.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setGeometry(0, 0, 1200, 600);
    int visuablePartsAmount = 10; //первые 10 значений графика

    //генерируем цвета для графиков
    for (int i = 0; i <= visuablePartsAmount + 1; i++){
        int r = rand()%255;
        int g = rand()%255;
        int b = rand()%255;
        int bw = (r+g+b)/3;
        coloredColors.push_back(QColor(r,g,b));
        blackWhiteColors.push_back(QColor(bw, bw, bw));
    }

    isChartOpen = false;

    //задаем расположение
    horizontalLayout=new QHBoxLayout(this);
    verticalLeftLayout = new QVBoxLayout();
    verticalRightLayout = new QVBoxLayout();
    graphSettingsLayout = new QHBoxLayout();
    horizontalLayout->addLayout(verticalLeftLayout);
    horizontalLayout->addLayout(verticalRightLayout);
    verticalRightLayout->addLayout(graphSettingsLayout);

    splitterLeft = new QSplitter;
    splitterRight = new QSplitter;
    verticalLeftLayout->addWidget(splitterLeft);
    verticalRightLayout->addWidget(splitterRight);


    //кнопка выбора папки
    directoryButton = new QPushButton ("Open directory");

    // модель представления для файлов
    tableModel =  new QFileSystemModel(this);
    //фильтры файловой модели
    tableModel->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    //изначальная отображаемая папка
    directoryName = QDir::rootPath();
    //изначальная открытая папка
    tableModel->setRootPath(directoryName);
    //табличное представления файлов
    tableView = new QTableView;
    //дружим представление с моделью
    tableView->setModel(tableModel);

    QItemSelectionModel *selectionModel = tableView->selectionModel();

    printChartButton = new QPushButton ("Print graph");
    checkboxBlackWhiteChart = new QCheckBox("B/w graph");

    chartTypeCombobox = new QComboBox();
    chartTypeCombobox->insertItem(1, QString("Pie chart"));
    chartTypeCombobox->insertItem(2, QString("Bar chart"));

    filePath = "";
    chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    //остальное реализуется в слотах выбора файла и типа графика

    //представление файлов в папке
    splitterLeft->addWidget(tableView);
    //кнопка выбора папки
    verticalLeftLayout->addWidget(directoryButton);

    //график
    splitterRight->addWidget(chartView);

    //кнопка печати
    graphSettingsLayout->addWidget(printChartButton);
    //галочка ч-б графика
    graphSettingsLayout->addWidget(checkboxBlackWhiteChart);
    //выбор типа графика
    graphSettingsLayout->addWidget(chartTypeCombobox);

    //открытие директории
    connect(directoryButton, SIGNAL(clicked()), this, SLOT(openDirectorySlot()));
    //печать графика
    connect(printChartButton, SIGNAL(clicked()), this, SLOT(printChartSlot()));
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(fileChooseSlot(const QItemSelection &, const QItemSelection &)));
    //изменение типа графика
    connect(chartTypeCombobox, SIGNAL (currentIndexChanged(int)), this,  SLOT(chartTypeChangeSlot()));
    //изменение цвета графика
    connect(checkboxBlackWhiteChart, SIGNAL(toggled(bool)), this, SLOT(recoloringChartSlot()));

}

void MainWindow::recoloringChartSlot()
{
    // если график открыт меняем цвет
    if (isChartOpen){
        if (checkboxBlackWhiteChart->checkState())
            chart->recolorChart(blackWhiteColors);
        else
            chart->recolorChart(coloredColors);
    }
}

void MainWindow::chartRepaint()
{
    //получаем график нужного типа
    chart = gContainer.GetObject<IChart>().get();

    //заполняем график считанными данными с помощью нужного читателя
    if (checkboxBlackWhiteChart->checkState())
        chart->createChart(gContainer.GetObject<IChartData>()->getData(filePath), blackWhiteColors);
    else
        chart->createChart(gContainer.GetObject<IChartData>()->getData(filePath), coloredColors);

    //меняем отображаемый график на новый
    chartView->setChart(chart->getChart());

    isChartOpen = true;
}

void MainWindow::chartTypeChangeSlot()
{
    QString chartType (chartTypeCombobox->currentText());
    bool chartTypeRight = false;
    //Сверяем тип графика, связываем нужный интерфейс с реализацией
    if (chartType == "Pie chart"){
        gContainer.RegisterInstance<IChart, PieChart>();
        chartTypeRight = true;
    }
    else if (chartType == "Bar chart"){
        gContainer.RegisterInstance<IChart, BarChart>();
        chartTypeRight = true;
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("Unknown chart type: " + chartType);
        messageBox.exec();
        isChartOpen = false;
    }

    //если все вводные в порядке, перерисовываем график
    if (chartTypeRight && !filePath.isEmpty() && isChartOpen)
        chartRepaint();
}

void MainWindow::openDirectorySlot()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec())
        directoryName = dialog.selectedFiles().first();;
    tableView->setRootIndex(tableModel->setRootPath(directoryName));
}

void MainWindow::printChartSlot()
{
    if (isChartOpen){
        QString saving_path("");

        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        if (dialog.exec())
            saving_path = dialog.selectedFiles().first();

        QPdfWriter* writer = new QPdfWriter (saving_path + "/out.pdf");

        //Указываем создателя документа
        writer->setCreator("user");
        //Устанавливаем размер страницы
        writer->setPageSize(QPageSize::A4);
        QPainter painter(writer);
        chartView->render(&painter);
        painter.end();
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("No chart to print");
        messageBox.exec();
    }
}

void MainWindow::fileChooseSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    bool fileFormatRight = false;
    bool chartTypeRight = false;

    //проверяем тип:
    QString chartType (chartTypeCombobox->currentText());
    if (chartType == "Pie chart"){
        gContainer.RegisterInstance<IChart, PieChart>();
        chartTypeRight = true;
    }
    else if (chartType == "Bar chart"){
        gContainer.RegisterInstance<IChart, BarChart>();
        chartTypeRight = true;
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("Unknown chart type: " + chartType);
        messageBox.exec();
    }

    //получаем путь:
    QModelIndexList indexs = selected.indexes();
    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = tableModel->filePath(ix);
    }

    //связываем интерфейс получения данных с конкретным читателем:
    if (filePath.endsWith(".sqlite")){
        gContainer.RegisterInstance<IChartData, ChartDataSqlite>();
        fileFormatRight = true;
    }

    else if (filePath.endsWith(".json")){
        gContainer.RegisterInstance<IChartData, ChartDataJSON>();
        fileFormatRight = true;
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("Unknown file type");
        messageBox.exec();
        isChartOpen = false;
    }

    if (fileFormatRight && chartTypeRight)
        chartRepaint();    //перерисовывем график
}

MainWindow::~MainWindow()
{

}
