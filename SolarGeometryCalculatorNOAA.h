#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_SolarGeometryCalculatorNOAA.h"
#include <QtWidgets>

#include "powercellgraphs.h"


class SolarGeometryCalculatorNOAA : public QMainWindow
{
   
Q_OBJECT
public:
	
    SolarGeometryCalculatorNOAA(QWidget *parent = nullptr);
    ~SolarGeometryCalculatorNOAA();
    QVector <QString> PathWithLatFolder, PathWithPowerFiles;
    QVector<QMap<QString, int>> AMStatData;
    QVector<double> SunRadVector;
    QString path2spec = ".//Data/BasicAM";
    QString BasicAM0 = ".//Data/AM0.dat";
    Ui::SolarGeometryCalculatorNOAAClass ui;
    void exitApplication()
    {
        QCoreApplication::quit();
    }
    QMap<QLineEdit*, QLabel*> statusLabelsMap;
    template < typename Templl >
    void ShowTableWidgetAmStatisticOrPower(QTableWidget* qtwidget, QVector<QMap<QString, Templl>>& AMData, QString ColName);
    PowerCellGraphs* graphsWindow = nullptr;
    
public slots:
    void validateDataSGCInput();
   
    void info();
    void Error();

    //void ShowTableWidgetAmStatisticOrPower1(QTableWidget* qtwidget, QVector<QMap<QString, int>>& AMData, QString ColName);

private slots:
    //void SolarSpectrumCalculator(bool isWithSC);
    //void onSpectrumButtonClicked() { SolarSpectrumCalculator(FALSE); };

    void CalcNOAA();
    bool CheckValidationTime(QLineEdit *lineEdit, QLabel* statusLabel);
    void validateDateTimeInput();
    //void CheckValidationInputSGC(QLineEdit* lineEdit, QLabel* statusLabel);
    bool CheckValidationLongitude();
    bool CheckValidationLatitude();
   // bool CheckValidationIntervalSec();

    bool areAllFieldsValid(const QList<bool>& fields) {
        for (const auto& field : fields) {
            if (!field) {
                return false;
            }
        }
        return true;
    }
    void ShowTableWidgetAmElev(const QVector<QVector<double>>& SGCData);
    void CheckRowTableWidget(QMap<QString, int> & frequencyMap, QString name_col, const double min, const double max,  const double precision);
    QVector<QMap<QString, int>>  GetAmStatistic();
	//void ShowTableWidgetAmStatistic(const QVector<QMap<QString, int>>& AMStatData);
     void SaveSGC();
    void SaveAMElevation(QTableWidget* table, int Var);

    void on_showGraphsButton_clicked();



};


template < typename Templl >
inline void SolarGeometryCalculatorNOAA::ShowTableWidgetAmStatisticOrPower(QTableWidget* qtwidget, QVector<QMap<QString, Templl>>& AMData, QString ColName)
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
                headers << ColName+"+Ref";
            }
       
        }

        qtwidget->setHorizontalHeaderLabels(headers);

        QStringList keys = AMData[0].keys();

        for (int row = 0; row < rowCount; ++row) {
            qtwidget->setItem(row, 0, new QTableWidgetItem(keys[row]));
        }
      
        for (int col = 0; col < colCount; ++col) {
            const QMap<QString, Templl>& map = AMData[col];
            for (int row = 0; row < rowCount; ++row) {
                QString key = keys[row];
                Templl value = map.value(key);

                if constexpr (std::is_same_v<Templl, int>) {
                    qtwidget->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
                }
                else if constexpr (std::is_same_v<Templl, double>) {
                    qtwidget->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
                }
            }
        }

        qtwidget->resizeColumnsToContents();
    }

}