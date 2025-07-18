
#include <iostream>

#include "SolarGeometryCalculatorNOAA.h"
#include "Solar_spectrum_calculator.h"

void Solar_spectrum_calculator::CheckDir()
{
    QDir dir1(".//");
    QString dirName = "Spectrum";
    if (!dir1.exists(dirName)) {
        dir1.mkpath(dirName);
    }

    QDir dir(".//Spectrum/");
    dirName = "Latitude";
    if (!dir.exists(dirName)) {
        dir.mkpath(dirName);
    }
}



bool Solar_spectrum_calculator::CheckValidationATPres()
{
    sgcnoaaWindow->ui.PressLineEdit->setPlaceholderText("800.0 : 1090.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(800.0, 1090.0, 7, sgcnoaaWindow->ui.PressLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    sgcnoaaWindow->ui.PressLineEdit->setValidator(lonValidator);
    lonValidator->setLocale(QLocale::C); // запрещаем запятые как разделитель
    logic->statusLabelsMap.insert(sgcnoaaWindow->ui.PressLineEdit, sgcnoaaWindow->ui.l_ATM_pressure);
    connect(sgcnoaaWindow->ui.PressLineEdit, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
    QString text = sgcnoaaWindow->ui.PressLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationWaterVapor()
{
    sgcnoaaWindow->ui.WLineEdit->setPlaceholderText("0.0 : 100.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(0.0, 100.0, 7, sgcnoaaWindow->ui.WLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    lonValidator->setLocale(QLocale::C); // запрещаем запятые как разделитель
    sgcnoaaWindow->ui.WLineEdit->setValidator(lonValidator);

    logic->statusLabelsMap.insert(sgcnoaaWindow->ui.WLineEdit, sgcnoaaWindow->ui.l_wather_vapor);
    connect(sgcnoaaWindow->ui.WLineEdit, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
    QString text = sgcnoaaWindow->ui.WLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationOzone()
{
    sgcnoaaWindow->ui.OZLineEdit->setPlaceholderText("0.0 : 1.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(0.0, 1., 7, sgcnoaaWindow->ui.OZLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    lonValidator->setLocale(QLocale::C); // запрещаем запятые как разделитель
    sgcnoaaWindow->ui.OZLineEdit->setValidator(lonValidator);
    logic->statusLabelsMap.insert(sgcnoaaWindow->ui.OZLineEdit, sgcnoaaWindow->ui.l_ozone);
    connect(sgcnoaaWindow->ui.OZLineEdit, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
    QString text = sgcnoaaWindow->ui.OZLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationTurbidityAOT()
{
    sgcnoaaWindow->ui.BetaLineEdit->setPlaceholderText("0.0 : 1.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(0.0, 1.0, 7, sgcnoaaWindow);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    lonValidator->setLocale(QLocale::C); // запрещаем запятые как разделитель
    sgcnoaaWindow->ui.BetaLineEdit->setValidator(lonValidator);
    logic->statusLabelsMap.insert(sgcnoaaWindow->ui.BetaLineEdit, sgcnoaaWindow->ui.l_turbidity);
    connect(sgcnoaaWindow->ui.BetaLineEdit, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
    QString text = sgcnoaaWindow->ui.BetaLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckIsASTM173()
{
    if (sgcnoaaWindow->ui.cb_isStandartATM->isChecked())
    {
        sgcnoaaWindow->ui.PressLineEdit->setText("1013.25");
        sgcnoaaWindow->ui.WLineEdit->setText("1.4164");
        sgcnoaaWindow->ui.OZLineEdit->setText("0.3438");
        sgcnoaaWindow->ui.BetaLineEdit->setText("0.084");
    }
    return sgcnoaaWindow->ui.cb_isStandartATM->isChecked();
}


//QVector<QMap<QString, double>>  Solar_spectrum_calculator::readPowerFileToVectorMap(const QString& filePath)
//{
//    QVector<QMap<QString, double>> dataVector;
//    QFile file(filePath);
//
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        QMap<QString, double> dataMap;
//
//        while (!in.atEnd()) {
//            QString line = in.readLine();
//            QStringList values = line.split('\t', Qt::SkipEmptyParts);
//            if (values.size() == 2) {
//                bool ok1, ok2;
//                double value1 = values[0].toDouble(&ok1); 
//                double value2 = values[1].toDouble(&ok2);
//                if (ok1 && ok2) {
//                    QString key = QString::number(value1, 'f', 2); 
//                    dataMap[key] = value2;
//                }
//                else {
//                    std::cerr << "Ошибка при преобразовании строки в double: "
//                        << line.toStdString() << std::endl;
//                }
//            }
//            else if (!line.trimmed().isEmpty()) {
//                std::cerr << "Неверное количество значений в строке: "
//                    << line.toStdString() << std::endl;
//            }
//        }
//        file.close();
//
//        // Вставляем одну карту в вектор
//        dataVector.append(dataMap);
//    }
//    else {
//        std::cerr << "Не удалось открыть файл: "
//            << filePath.toStdString() << std::endl;
//    }
//
//    return dataVector;
//}
