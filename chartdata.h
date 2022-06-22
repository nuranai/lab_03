#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QtSql>
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

//пара ключ-значение
struct DataStorage
{
public:
    QString key;
    double value;
    DataStorage (QString key_, double value_) { key = key_; value = value_; };
};

//интерфейс получения данных из разных источников
class IChartData
{
public:
// получаем данные из источника
    virtual QVector <DataStorage> getData (QString path_) = 0;
};



//для Sqlite-файлов
class ChartDataSqlite : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

//для JSON-файлов
class ChartDataJSON: public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};
#endif // CHARTDATA_H
