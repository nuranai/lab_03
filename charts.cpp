#include "charts.h"

void PieChart::createChart(QVector <DataStorage> data, QList <QColor> colors)
{
    chart->setTitle(title);

    series = new QPieSeries ();

    int i = 0;
    foreach (DataStorage elem, data) {
        QString legendHeader (elem.key);
        series->append(legendHeader, elem.value);
        series->slices().at(i)->setBrush(colors.at(i));
        i++;
    }

    chart->removeAllSeries();
    chart->addSeries(series);
}

void BarChart::createChart(QVector<DataStorage> data, QList <QColor> colors)
{
    chart->setTitle(title);

    series = new QBarSeries();

    int i = 0;
    foreach (DataStorage elem, data) {
        QString legendHeader (elem.key);
        QBarSet *set = new QBarSet(legendHeader);
        *set << elem.value;
        set->setBrush(QColor(colors.at(i)));
        series->append(set);
        i++;
    }

    chart->removeAllSeries();
    chart->addSeries(series);
}

void PieChart::recolorChart(QList <QColor> colors)
{
    int i=0;
    foreach (QPieSlice* x, series->slices()){
        x->setBrush(colors.at(i));
        i++;
    }
}

void BarChart::recolorChart(QList <QColor> colors)
{
    int i=0;
    foreach (QBarSet* x, series->barSets()){
        x->setBrush(colors.at(i));
        i++;
    }
}

