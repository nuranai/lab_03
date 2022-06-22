#ifndef CHARTS_H
#define CHARTS_H

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QJsonDocument>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

#include "chartdata.h"

 //интерфейс графика
class IChart
{
public:
    QString title;
    //функция построения графика. конкретная реализация - в конкретном графике.
    //аргументы: отсортированный вектор данных, цвета раскраски
    virtual void createChart (QVector <DataStorage>, QList <QColor> colors) = 0;

    virtual void recolorChart (QList <QColor>) = 0; //перекрасить цветами списка
    virtual QChart* getChart() = 0;
    virtual ~IChart() {};
};

// тип графика Pie
class PieChart : public IChart
{
protected:
    QChart *chart;
    QPieSeries *series;
public:
    PieChart() {
        title = "Pie chart";
        chart = new QChart;
    };
    QChart* getChart() {return chart;};
    void createChart (QVector <DataStorage>, QList <QColor> colors);
    void recolorChart (QList <QColor>);
    ~PieChart() {
    };
};

// тип графика Bar
class BarChart : public IChart
{
protected:
    QChart *chart;
    QBarSeries *series;
public:
    BarChart() {
        title = "Bar chart";
        chart = new QChart;
    };
     QChart* getChart() {return chart;};
    void createChart (QVector <DataStorage>, QList <QColor> colors);
    void recolorChart (QList <QColor>);
    ~BarChart() {
      };
};
#endif // CHARTS_H
