#include "stdafx.h"
#include "SolarGeometryCalculatorNOAA.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SolarGeometryCalculatorNOAA window;
    window.show();
    return app.exec();
}
