
#include "SolarGeometryCalculatorNOAA.h"
#include <QtWidgets/QApplication>

#include "SolarCell.h"
#include "Solar_spectrum_calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SolarGeometryCalculatorNOAA window;
   // Solar_spectrum_calculator ssc;
    //SolarCell sc;
    window.show();
    //sc.show(); ssc.show();
    return app.exec();
}
