#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_SolarGeometryCalculatorNOAA.h"
#include <QtWidgets>


class SolarGeometryCalculatorNOAA : public QMainWindow
{
    Q_OBJECT

public:
    SolarGeometryCalculatorNOAA(QWidget *parent = nullptr);
    ~SolarGeometryCalculatorNOAA();

    
private:
    Ui::SolarGeometryCalculatorNOAAClass ui;
    QMap<QLineEdit*, QLabel*> statusLabelsMap;

    struct isControl
    {
	    
    };


private slots:

    void InputData();
    bool CheckValidationTime(QLineEdit *lineEdit, QLabel* statusLabel);
    void validateDateTimeInput();
    //void CheckValidationInputSGC(QLineEdit* lineEdit, QLabel* statusLabel);
    bool CheckValidationLongitude();
    bool CheckValidationLatitude();
    bool CheckValidationIntervalSec();
    void validateDataSGCInput();
    bool areAllFieldsValid(const QList<bool>& fields) {
        for (const auto& field : fields) {
            if (!field) {
                return false;
            }
        }
        return true;
    }
    void ShowTableWidget(const QVector<QVector<double>> SGCData);
};


