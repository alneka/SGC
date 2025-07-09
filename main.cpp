
#include "SolarGeometryCalculatorNOAA.h"
#include <QtWidgets/QApplication>

#include "Solar_spectrum_calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SolarGeometryCalculatorNOAA window;
    Solar_spectrum_calculator ssc;
    //window.show();
    ssc.show();
    return app.exec();
}
