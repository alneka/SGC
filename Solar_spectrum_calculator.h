#pragma once

#include <QtWidgets/QMainWindow>

#include "SolarGeometryCalculatorNOAA.h"
#include "ui_SolarGeometryCalculatorNOAA.h"

#include "Spectr_gen.h"


class Solar_spectrum_calculator : public QObject
{
    Q_OBJECT

public:
    Solar_spectrum_calculator(SolarCellPowerLogic* logic, SolarGeometryCalculatorNOAA* sgcnoaaWindow) : logic(logic), sgcnoaaWindow(sgcnoaaWindow)
    {
        CheckValidationATPres();
        CheckValidationWaterVapor();
        CheckValidationOzone();
        CheckValidationTurbidityAOT();
        connect(sgcnoaaWindow->ui.pb_GetSpectrum, SIGNAL(clicked()), this, SLOT(CalcSolarSpectrum()));
    }
    ~Solar_spectrum_calculator();
    //QVector <QString> PathWithLatFolder, PathWithPowerFiles;

    QString BasicAM0 = ".//Data/AM0.dat";
   
    
    QVector<QString> Lat_name = {"9080","8070","7060","6050","5040","4030","3020", "2010","1000",
        "0010","1020","2030","3040","4050","5060","6070","7080","8090"};
	
   // QString path2spec = ".//Data/BasicAM";
    //QString BasicAM0 = ".//Data/AM0.dat";
  void CalcAMPowerATM( QString _Press, QString _W, QString _OZ, QString _D, QString _Beta, int VAR_CALC, QVector<double> Beta_vec);
private:
    SolarCellPowerLogic* logic;
    SolarGeometryCalculatorNOAA* sgcnoaaWindow;

private slots:
    void CalcSolarSpectrum();
    void GetPowerAMPreriod(QVector<QString>& Path_to_power);
    //Ui::SolarGeometryCalculatorNOAAClass ui;
    void CheckDir();
  
    bool CheckValidationATPres();
    bool CheckValidationWaterVapor();
    bool CheckValidationOzone();
    bool CheckValidationTurbidityAOT();

    bool CheckIsASTM173();
    //QVector<QMap<QString, double>> readPowerFileToVectorMap(const QString& filePath);
    //void ShowTableWidgetPowerAmPeriod(const QVector<QMap<QString, int>>& PowerPeriodData);
    void SaveSpec(double BetaPar, double AMpar, QVector<double> Spectr, double * AM0_WL);
    void SavePowerAM(double BetaPar, QVector<double> v_am, QVector<double> v_power);
   // void info();
    //void Error();


};


