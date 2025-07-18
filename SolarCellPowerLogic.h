#pragma once
#include <iostream>
#include <QFile>
#include <QLabel>

#include "C:\Qt\qt\6.4.1\msvc2019_64\include\QtCore\qobject.h"
#include <QtWidgets/qtablewidget.h>

#include "PNTransition.h"


class SolarCellPowerLogic :
    public QObject
{
    Q_OBJECT
public:
    SolarCellPowerLogic(QObject* parent = nullptr);
    SolarCellPowerLogic() : QObject(nullptr) {} // по умолчанию
    ~SolarCellPowerLogic() override;
    QVector<QMap<QString, int>> AMStatData;
    QVector<double> SunRadVector;   
    QMap<QLineEdit*, QLabel*> statusLabelsMap;
    QVector <QString> PathWithLatFolder, PathWithPowerFiles;
    QVector<QString> Path_to_solar_spec;
    QVector<QString> Path_to_power;
    QVector<QPair<int, int>> minMaxLambdaPN;
    //QVector<QMap<QString, int>> AMStatData;
    //QVector<double> SunRadVector;
    QString Path = ".//Spectrum/";
    QString path2spec = ".//Data/BasicAM";
    QString BasicAM0 = ".//Data/AM0.dat";
    PN all_pn;
    int n_pn = 0;
   QVector<IntDoubleStruct> vecSolarCellPN[6];
    template < typename Templll >
    void ShowTableWidgetAmStatisticOrPower(QTableWidget* qtwidget, QVector<QMap<QString, Templll>>& AMData, QString ColName);
    template < typename TemplRead >
    void Read2DFile(QString filePath, QVector<TemplRead>& vectorData);

public slots:
    void GetDefaultAMStatData();
    void validateDataInput();
    bool GetSolarCellSpectrum();
    void info();
    void Error();

};

template < typename Templll >
inline void SolarCellPowerLogic::ShowTableWidgetAmStatisticOrPower(QTableWidget* qtwidget, QVector<QMap<QString, Templll>>& AMData, QString ColName)
{
    {
        if (AMData.isEmpty())
            return;

        const int rowCount = AMData[0].size();
        const int colCount = AMData.size();

        qtwidget->clear();
        qtwidget->setRowCount(rowCount);
        qtwidget->setColumnCount(colCount + 1);

        QStringList headers;
        headers << "N";
        if (colCount > 0)
        {
          
            if (colCount > 1)
            {
                for (int i = 1; i < colCount+1; i++)
                {
                    headers << ColName + QString::number(i);
                }
                
            }
            else  headers << QString(ColName);

        }

        qtwidget->setHorizontalHeaderLabels(headers);

        QStringList keys = AMData[0].keys();

        for (int row = 0; row < rowCount; ++row) {
            qtwidget->setItem(row, 0, new QTableWidgetItem(keys[row]));
        }

        for (int col = 0; col < colCount; ++col) {
            const QMap<QString, Templll>& map = AMData[col];
            for (int row = 0; row < rowCount; ++row) {
                QString key = keys[row];
                Templll value = map.value(key);

                if constexpr (std::is_same_v<Templll, int>) {
                    qtwidget->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
                }
                else if constexpr (std::is_same_v<Templll, double>) {
                    qtwidget->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
                }
            }
        }

        qtwidget->resizeColumnsToContents();
    }

}


template < typename TemplRead > //  QVector<QMap<QString, double>> or QVector<IntDoubleStruct>>
inline void SolarCellPowerLogic::Read2DFile(QString filePath, QVector<TemplRead>& vectorData)
{
   // QVector<IntDoubleStruct> dataFile;
    //QVector<QMap<QString, double>> dataVector;

    QFile file(filePath);
   
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Не удалось открыть файл: "
            << filePath.toStdString() << std::endl;
        return;
    }

    QTextStream in(&file);
    QMap<QString, double> dataMap;
    while (!in.atEnd()) {
        IntDoubleStruct dataIntDouble;
        QString line = in.readLine();
        QStringList values = line.split('\t', Qt::SkipEmptyParts);
        if (values.size() == 2) {
        bool ok1, ok2;
            if constexpr (std::is_same_v<TemplRead,  IntDoubleStruct>)
            {
                int value1 = values[0].toInt(&ok1);
                double value2 = values[1].toDouble(&ok2);
                if (ok1 && ok2 && value1 != 0) {

                    dataIntDouble.intData = value1;
                    dataIntDouble.doubleData = value2;
                }
            }
            else if constexpr (std::is_same_v<TemplRead, QMap<QString, double>>)
            {
                double value1 = values[0].toDouble(&ok1);
                double value2 = values[1].toDouble(&ok2);
                if (ok1 && ok2 && value1 != 0) {
                    QString key = QString::number(value1, 'f', 2);
                    dataMap[key] = value2;
                }
            }

            
            else {
                std::cerr << "Ошибка при преобразовании строки в int/double: "
                    << line.toStdString() << std::endl;
            }
        }
        else if (!line.trimmed().isEmpty()) {
            std::cerr << "Неверное количество значений в строке: "
                << line.toStdString() << std::endl;
        }
        if constexpr (std::is_same_v<TemplRead, IntDoubleStruct>)
        {
            // Вставляем одну карту в вектор
            if(dataIntDouble.intData != 0 && dataIntDouble.doubleData != 0.0)
            vectorData.push_back(dataIntDouble);
        }
    }
    file.close();
    if constexpr (std::is_same_v<TemplRead, QMap<QString, double>>)
    {
        vectorData.append(dataMap);
    }


   
}