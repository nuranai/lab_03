#include "chartdata.h"
#include <QMessageBox>


QVector <DataStorage> ChartDataSqlite::getData (QString path_)
{
    //вектор заголвков и счетчиков
    QVector <DataStorage> data;

    //создаем соединение по умолчанию с драйвером "QSQLITE"
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(path_);

    //открываем, проверяем на открытие
    if (!dbase.open()) {
        QMessageBox messageBox;
        messageBox.setText("Can`t open database " + path_);
        messageBox.exec();
    }

    //собираем данные из базы в data
    else {
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
        int i = 0;
        while (query.next() && i < 10) {
            i++;
            QString key = query.value(0).toString();
            double value = query.value(1).toDouble();
            data.push_back(DataStorage(key, value));
        }
    }

    return data;
}

QVector <DataStorage> ChartDataJSON::getData(QString path_)
{
    //вектор заголвков и счетчиков
    QVector <DataStorage> data;

    QString fileInString;
    QFile file;
    file.setFileName(path_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox messageBox;
        messageBox.setText("Can't open json file " + path_);
        messageBox.exec();
    }
    fileInString = file.readAll();
    file.close();
    // конвертируем в JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileInString.toUtf8());
    QJsonObject jsonObject = doc.object();
    // получаем список ключей
    QStringList keys = jsonObject.keys();
    QListIterator<QString> iterator(keys);
    int i = 0;

    while (iterator.hasNext() && i < 10) {
        QString key = iterator.next();
        double value = jsonObject.value(key).toDouble();
        data.push_back(DataStorage(key, value));
    }

    return data;
}
