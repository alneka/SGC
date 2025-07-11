#pragma once
#include <QLabel>

#include "C:\Qt\qt\6.4.1\msvc2019_64\include\QtCore\qobject.h"
#include <QtWidgets/qtablewidget.h>



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
    QVector<QPair<int, int>> minMaxLambdaPN;
    //QVector<QMap<QString, int>> AMStatData;
    //QVector<double> SunRadVector;
    QString path2spec = ".//Data/BasicAM";
    QString BasicAM0 = ".//Data/AM0.dat";
    template < typename Templll >
    void ShowTableWidgetAmStatisticOrPower(QTableWidget* qtwidget, QVector<QMap<QString, Templll>>& AMData, QString ColName);
    //template < typename TemplRead >
    //void Read2DFile(QString filePath, QVector<TemplRead>& vectorData);
public slots:

    void validateDataInput();

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
            headers << QString(ColName);
            if (colCount > 1)
            {
                headers << ColName + "+Ref";
            }

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

//
//template < typename TemplRead > //  QVector<QMap<QString, double>> or QVector<IntDoubleStruct>>
//inline void SolarCellPowerLogic::Read2DFile(QString filePath, QVector<TemplRead>& vectorData)
//{
//   // QVector<IntDoubleStruct> dataFile;
//    //QVector<QMap<QString, double>> dataVector;
//
//    QFile file(filePath);
//    if constexpr (std::is_same_v<TemplRead, QMap<QString, double>>)
//    {
//	    
//    }
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        QMap<QString, double> dataMap;
//        while (!in.atEnd()) {
//            IntDoubleStruct dataIntDouble;
//            QString line = in.readLine();
//            QStringList values = line.split('\t', Qt::SkipEmptyParts);
//            if (values.size() == 2) {
//                bool ok1, ok2;
//                int value1 = values[0].toInt(&ok1);
//                double value2 = values[1].toDouble(&ok2);
//                if (ok1 && ok2) {
//                    if constexpr (std::is_same_v<TemplRead, QVector <IntDoubleStruct>>)
//                    {
//                        dataIntDouble.intData = value1;
//                        dataIntDouble.doubleData = value2;
//                    }
//                    if constexpr (std::is_same_v<TemplRead, QMap<QString, double>>)
//                    {
//                        QString key = QString::number(value1, 'f', 2);
//                        dataMap[key] = value2;
//                    }
//
//                }
//                else {
//                    std::cerr << "Ошибка при преобразовании строки в int/double: "
//                        << line.toStdString() << std::endl;
//                }
//            }
//            else if (!line.trimmed().isEmpty()) {
//                std::cerr << "Неверное количество значений в строке: "
//                    << line.toStdString() << std::endl;
//            }
//            if constexpr (std::is_same_v<TemplRead, QVector <IntDoubleStruct>>)
//            {
//                // Вставляем одну карту в вектор
//                vectorData.push_back(dataMap);
//            }
//        }
//        file.close();
//        if constexpr (std::is_same_v<TemplRead, QMap<QString, double>>)
//        {
//            vectorData.append(dataMap);
//        }
//    }
//    else {
//        std::cerr << "Не удалось открыть файл: "
//            << filePath.toStdString() << std::endl;
//    }
//
//    return dataFile;
//}