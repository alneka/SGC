#include "Solar_spectrum_calculator.h"

#include <iostream>
#include <QFileDialog>



Solar_spectrum_calculator::~Solar_spectrum_calculator()
{}


void Solar_spectrum_calculator::CalcSolarSpectrum()
{
    QVector<bool> vec_validate = {  CheckValidationATPres(),
     CheckValidationWaterVapor(),
     CheckValidationOzone(),
     CheckValidationTurbidityAOT(),
    };
    if (vec_validate.contains(false) && !CheckIsASTM173()) { return; }
    //ui.textBrowser->clear();
    void CheckDir();
    //CheckIsASTM173();
    logic->PathWithLatFolder.clear();
    logic->PathWithPowerFiles.clear();
    //QString _AM = ui.aMLineEdit->text();
    QString _Press = sgcnoaaWindow->ui.PressLineEdit->text();
    QString _W = sgcnoaaWindow->ui.WLineEdit->text();
    QString _OZ = sgcnoaaWindow->ui.OZLineEdit->text();
    QString _D = "1";// ui.DLineEdit->text();
    QString _Beta = sgcnoaaWindow->ui.BetaLineEdit->text();

    bool control = false;
    if (_Press.isEmpty() || _W.isEmpty(), _OZ.isEmpty() || _D.isEmpty() || _Beta.isEmpty(), logic->BasicAM0.isEmpty()) {
        logic->Error();  return;
    }

    int VAR_CALC = 0;
    //if (ui.OneAM_cbox->isChecked()) {
    //    VAR_CALC = 1;
    //    if (_AM.isEmpty()) control = TRUE;
    //}
    QVector<double> Beta_vec;
    QVector<double> Latitudes;
    //if (ui.AOT_cbox->isChecked()) {
    //    GetVectorOfBeta(Beta_vec, Latitudes);
    //    VAR_CALC = 2;
    //}
    QString path2spec_reserv = logic->path2spec;

    /// <summary>
    /// Spectrum calculate
    /// </summary>
    /// <param name="isWithSC"></param>
    //AddTextInTextBrowser("Calculate AM is starting (data for user Spectrum)");
    ///Solar_spectrum_calculator SSC(logic, this);;
    //SSC.BasicAM0 = logic->BasicAM0;
    this->BasicAM0 = logic->BasicAM0;
    CalcAMPowerATM(_Press, _W, _OZ, _D, _Beta, VAR_CALC, Beta_vec);
    logic->path2spec = logic->Path_to_solar_spec[0];
    //ui.label_22->setText(path2spec);
   // AddTextInTextBrowser("Get AM data for user Spectrum");
    if (logic->AMStatData.isEmpty()) logic->GetDefaultAMStatData();
	if (!logic->AMStatData.isEmpty()) GetPowerAM(logic->Path_to_power);
}




void Solar_spectrum_calculator::CalcAMPowerATM( QString _Press, QString _W, QString _OZ, QString _D, QString _Beta, int VAR_CALC, QVector<double> Beta_vec)
{
    CheckDir();
    double D = _D.toDouble();
    double Press = _Press.toDouble();
    double BetaPar = _Beta.toDouble();
    double W = _W.toDouble();
    double OZ = _OZ.toDouble();
    GetSpectAM0andAlpha(BasicAM0);
    Set_AM_Power_beta(BetaPar, 1.0, 10.0, 0.01, D, Press, W, OZ, logic->Path);
    logic->Path_to_solar_spec.push_back(logic->Path + "M" + QString::number(BetaPar));
    logic->Path_to_power.push_back(logic->Path + "M" + QString::number(BetaPar) + "/" + "M" + QString::number(BetaPar) + "power" + ".dat");
	
}






void Solar_spectrum_calculator::GetPowerAM(QVector<QString>& Path_to_power)
{
   //QVector<QMap<QString, double>> power_from_file = readPowerFileToVectorMap(Path_to_power[0]);
   QVector<QMap<QString, double>> power_from_file;
	logic->Read2DFile(Path_to_power[0], power_from_file);

    QVector<QMap<QString, double>> result;

    
    for (int mapIndex = 0; mapIndex < logic->AMStatData.size(); ++mapIndex)
    {
        const QMap<QString, int>& dataAMSD = logic->AMStatData[mapIndex];
        const QMap<QString, double>& dataPFF = power_from_file[0];
        QMap<QString, double> resultMap;
        if (power_from_file[0].size() != dataAMSD.size()) return;
        // Для каждого ключа в AMStatData ищем значение в power_from_file
        for (auto it = dataAMSD.constBegin(); it != dataAMSD.constEnd(); ++it) {
            const QString& key = it.key();
            int am_count = it.value();

            if (dataPFF.contains(key)) {
                double power = dataPFF.value(key);
                resultMap[key] = power;// *am_count;
            }
            else {
                std::cerr << "Ключ " << key.toStdString()
                    << " отсутствует в power_from_file!\n";
                resultMap[key] = 0.0; // или оставить 0, если ключ не найден
            }
        }

        result.append(resultMap);
    }
    logic->ShowTableWidgetAmStatisticOrPower(sgcnoaaWindow->ui.tw_AMPowerATM, result, QString("Power"));

}





//void Solar_spectrum_calculator::ShowTableWidgetPowerAmPeriod(const QVector<QMap<QString, int>>& PowerPeriodData)
//{
//    if (PowerPeriodData.isEmpty())
//        return;
//
//    // Все QMap имеют одинаковые ключи
//    const int rowCount = PowerPeriodData[0].size(); // количество уникальных AM значений
//    const int colCount = PowerPeriodData.size();    // количество наборов статистики
//
//    ui.tw_AMPowerATM->clear();
//    ui.tw_AMPowerATM->setRowCount(rowCount);
//    ui.tw_AMPowerATM->setColumnCount(colCount + 1); // +1 — столбец для ключей (AM)
//
//    // Заголовки столбцов
//    QStringList headers;
//    headers << "N";
//    if (colCount > 0)
//    {
//        headers << QString("AM");
//        if (colCount > 1)
//        {
//            headers << QString("Power");
//        }
//    }
//
//
//    ui.tw_AMPowerATM->setHorizontalHeaderLabels(headers);
//
//    // Получим упорядоченные ключи
//    QStringList keys = PowerPeriodData[0].keys();
//
//    // Заполняем первую колонку (AM значения)
//    for (int row = 0; row < rowCount; ++row) {
//        ui.tw_AMPowerATM->setItem(row, 0, new QTableWidgetItem(keys[row]));
//    }
//
//    // Заполняем значения статистики
//    for (int col = 0; col < colCount; ++col) {
//        const QMap<QString, int>& map = PowerPeriodData[col];
//        for (int row = 0; row < rowCount; ++row) {
//            QString key = keys[row];
//            int value = map.value(key, 0); // безопасно, даже если нет ключа
//            ui.tw_AMPowerATM->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
//        }
//    }
//
//    ui.tw_AMPowerATM->resizeColumnsToContents();
//}