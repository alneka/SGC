
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
    ui.PressLineEdit->setPlaceholderText("800.0 : 1090.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(800.0, 1090.0, 7, ui.PressLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    ui.PressLineEdit->setValidator(lonValidator);
    statusLabelsMap.insert(ui.PressLineEdit, ui.l_ATM_pressure);
    connect(ui.PressLineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
    QString text = ui.PressLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationWaterVapor()
{
    ui.WLineEdit->setPlaceholderText("800.0 : 1090.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(800.0, 1090.0, 7, ui.PressLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    ui.WLineEdit->setValidator(lonValidator);
    statusLabelsMap.insert(ui.WLineEdit, ui.l_wather_vapor);
    connect(ui.WLineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
    QString text = ui.WLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationOzone()
{
    ui.OZLineEdit->setPlaceholderText("800.0 : 1090.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(800.0, 1090.0, 7, ui.PressLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    ui.OZLineEdit->setValidator(lonValidator);
    statusLabelsMap.insert(ui.OZLineEdit, ui.l_ozone);
    connect(ui.OZLineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
    QString text = ui.OZLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

bool Solar_spectrum_calculator::CheckValidationTurbidityAOT()
{
    ui.BetaLineEdit->setPlaceholderText("800.0 : 1090.0");
    QDoubleValidator* lonValidator = new QDoubleValidator(800.0, 1090.0, 7, ui.PressLineEdit);
    lonValidator->setNotation(QDoubleValidator::StandardNotation);
    ui.BetaLineEdit->setValidator(lonValidator);
    statusLabelsMap.insert(ui.BetaLineEdit, ui.l_turbidity);
    connect(ui.BetaLineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
    QString text = ui.BetaLineEdit->text();
    int pos = 0;
    return lonValidator->validate(text, pos) == QValidator::Acceptable;
}

void Solar_spectrum_calculator::CheckIsASTM173()
{
    if (ui.cb_isStandartATM->isChecked())
    {
        ui.PressLineEdit->setText("1013.25");
        ui.WLineEdit->setText("1.4164");
        ui.OZLineEdit->setText("0.3438");
        ui.BetaLineEdit->setText("0.084");
    }
}


QVector<QMap<QString, double>>  Solar_spectrum_calculator::readPowerFileToVectorMap(const QString& filePath)
{
    QVector<QMap<QString, double>> dataVector;
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QMap<QString, double> dataMap;

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split('\t', Qt::SkipEmptyParts);
            if (values.size() == 2) {
                bool ok1, ok2;
                double value1 = values[0].toDouble(&ok1); 
                double value2 = values[1].toDouble(&ok2);
                if (ok1 && ok2) {
                    QString key = QString::number(value1, 'f', 2); 
                    dataMap[key] = value2;
                }
                else {
                    std::cerr << "Ошибка при преобразовании строки в double: "
                        << line.toStdString() << std::endl;
                }
            }
            else if (!line.trimmed().isEmpty()) {
                std::cerr << "Неверное количество значений в строке: "
                    << line.toStdString() << std::endl;
            }
        }
        file.close();

        // Вставляем одну карту в вектор
        dataVector.append(dataMap);
    }
    else {
        std::cerr << "Не удалось открыть файл: "
            << filePath.toStdString() << std::endl;
    }

    return dataVector;
}
