#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SolarGeometryCalculatorNOAA.h"

class SolarGeometryCalculatorNOAA : public QMainWindow
{
    Q_OBJECT

public:
    SolarGeometryCalculatorNOAA(QWidget *parent = nullptr);
    ~SolarGeometryCalculatorNOAA();

private:
    Ui::SolarGeometryCalculatorNOAAClass ui;
};

