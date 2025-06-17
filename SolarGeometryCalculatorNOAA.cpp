
#include "SolarGeometryCalculatorNOAA.h"

#include "SGCNOAA.h"
#include <QRegularExpressionValidator>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


#pragma execution_character_set("")

SolarGeometryCalculatorNOAA::SolarGeometryCalculatorNOAA(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //QRegularExpression rx("^([0-9]{4}) (0[1-9]|1[0-2]) (0[1-9]|[12][0-9]|3[01]) (0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$");
    //QRegularExpressionValidator* regexValidator = new QRegularExpressionValidator(rx, this); // 'this' должен быть QObject*
    CheckValidationTime(ui.le_UTC_Start_Date, ui.l_UTC_Start_Date);
    CheckValidationTime(ui.le_UTC_End_Date, ui.l_UTC_End_Date);
    CheckValidationLongitude();
    CheckValidationLatitude();
    //CheckValidationIntervalSec();
    // Предполагается, что ui.le_UTC_Start_Date - это QLineEdit*
    //ui.le_UTC_Start_Date->setValidator(regexValidator);
    //ui.le_UTC_End_Date->setValidator(regexValidator);

    connect(ui.pb_SGC_start, SIGNAL(clicked()), this, SLOT(InputData()));
    connect(ui.pb_SGC_AM_Elev_save, SIGNAL(clicked()), this, SLOT(SaveSGC()));
    //connect(ui.le_UTC_Start_Date, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::CheckValidationTime(*this));
}

SolarGeometryCalculatorNOAA::~SolarGeometryCalculatorNOAA()
{}

void SolarGeometryCalculatorNOAA::InputData() {

    QVector<bool> vec_validate = { CheckValidationTime(ui.le_UTC_Start_Date, ui.l_UTC_Start_Date),
    	CheckValidationTime(ui.le_UTC_End_Date, ui.l_UTC_End_Date),
		CheckValidationLongitude(),
		CheckValidationLatitude(),
		//CheckValidationIntervalSec()
		};
    if (!ui.cB_isElevation->isChecked() && !ui.cB_isElevation->isChecked() && !ui.cB_isElevation_with_refr->isChecked() && !ui.cB_isElevation_with_refr->isChecked()) { return; }
    if(vec_validate.contains(false)) { return; }
   //if()
    //{
        // 1. Получаем строку даты из пользовательского интерфейса
        QString starDateTimeString = ui.le_UTC_Start_Date->text();
        QString endDateTimeString = ui.le_UTC_End_Date->text();
        // 2. Определяем формат строки даты и преобразуем её в QDateTime
        QString format = "yyyy-MM-dd HH:mm";
        QDateTime starDateTime = QDateTime::fromString(starDateTimeString, format);
        QDateTime endDateTime = QDateTime::fromString(endDateTimeString, format);
        if (starDateTime > endDateTime) { return; }
        // 3. Считываем широту, долготу и интервал времени
        int longitude = ui.le_longitude->text().toInt();
        int latitude = ui.le_latitude->text().toInt();
        //int interval = ui.le_interval_time->text().toInt();
        int interval = 1; // 1 min 
        // 4. Создаём объект SGCNOAA с введёнными параметрами
        SGCNOAA sgcnoaa(starDateTime, endDateTime, latitude, longitude, interval);

        sgcnoaa.getResult();
        
        //QVector<double> vec_SEA = sgcnoaa.getSEA();
        QVector<QVector<double>> SGC ;
		if(ui.cb_isAM->isChecked())
		{
            SGC.push_back(sgcnoaa.getAM());
		}

        if (ui.cB_isElevation->isChecked())
        {
            SGC.push_back(sgcnoaa.getSEA());
        }
        if (ui.cb_isAM_with_refr->isChecked())
        {
            SGC.push_back(sgcnoaa.getAMC());
        }
        if (ui.cB_isElevation_with_refr->isChecked())
        {
            SGC.push_back(sgcnoaa.getSECFATMR());
        }


        //SGC.push_back(vec_AM);
        ShowTableWidgetAmElev(SGC);
        QVector<QMap<QString, int>> AMStatData;
        AMStatData = GetAmStatistic();
        ShowTableWidgetAmStatistic(AMStatData);
 /*       QStandardItemModel* model = new QStandardItemModel();

        for (double value : vec_AM) {
            QStandardItem* item = new QStandardItem(QString::number(value, 'f', 2));
            model->appendRow(item);
        }

        ui.listView->setModel(model);*/
    //};

    //Data data;
    //InData ID;

    //QString _day = ui.le_day->text();
    //QString _month = ui.le_month->text();
    //QString _year = ui.le_year->text();
    //QString _long = ui.le_long->text();
    //QString _latit = ui.le_latit->text();
    //QString _tzone = ui.le_timezone->text();

    //data.Day = _day.toInt();
    //data.Month = _month.toInt();
    //data.Year = _year.toInt();
    //ID.data = data;
    //ID.latitude = _latit.toInt();
    //ID.longitude = _long.toInt();
    //ID.timezone = _tzone.toInt();
    //QVector<double> TPL;
    //QVector<double> julian_days;
    //QVector<double> julian_century;
    //QVector<double> GMLS;
    //QVector<double> GMAS;
    //QVector<double> EEO;
    //QVector<double> SEoC;
    //QVector<double> STL;
    //QVector<double> STA;
    //QVector<double> SRV;
    //QVector<double> SAL;
    //QVector<double> MOE;
    //QVector<double> OC;
    //QVector<double> SRA;
    //QVector<double> SD;
    //QVector<double> VY;
    //QVector<double> EOT;
    //QVector<double> HAS;
    //QVector<double> SN;
    //QVector<double> SRT;
    //QVector<double> SST;
    //QVector<double> SLD;
    //QVector<double> TST;
    //QVector<double> HA;
    //QVector<double> SZA;
    //QVector<double> SEA;
    //QVector<double> AAR;
    //QVector<double> SECFATMR;
    //QVector<double> SAA;
    ////QVector<double>* mass[54] = { &TPL, &julian_days, &julian_century,
    ////    &GMLS, &GMAS, &EEO, &SEoC, &STL, &STA, &SRV, &SAL, &MOE, 
    ////    &OC, &SRA, &SD, &VY, &EOT, &HAS, &SN, &SRT, &SST, &SLD, 
    ////    &TST, &HA, &SZA, &SEA, &AAR, &SECFATMR, &SAA};


    //TimePastLocal(TPL);
    //JulianDay(julian_days, ID.data, TPL);
    //JulianCentury(julian_century, julian_days);
    //GeomMeanLongSun(julian_century, GMLS);
    //GeomMeanAnomSun(julian_century, GMAS);
    //EccentEarthOrbit(julian_century, EEO);
    //SunEqOfCtr(julian_century, GMAS, SEoC);
    //SunTrueLong(GMLS, SEoC, STL);
    //SunTrueAnom(GMAS, SEoC, STA);
    //SunRadVector(EEO, STA, SRV);
    //SunAppLong(julian_century, STL, SAL);
    //MeanObligEcliptic(julian_century, MOE);
    //ObliqCorr(julian_century, MOE, OC);
    //SunRtAscen(OC, SAL, SRA);
    //SunDeclin(SAL, OC, SD);
    //VarY(OC, VY);
    //EqOfTime(GMLS, GMAS, EEO, VY, EOT);
    //HASunrise(ID.latitude, SD, HAS);
    //SolarNoon(ID.longitude, ID.timezone, EOT, SN);
    //SunriseTime(SN, HAS, SRT);
    //SunsetTime(SN, HAS, SST);
    //SunlightDuration(HAS, SLD);
    //TrueSolarTime(TPL, EOT, ID.longitude, ID.timezone, TST);
    //HourAngle(TST, HA);
    //SolarZenitAngle(ID.latitude, SD, HA, SZA);
    //SolarElevationAngle(SZA, SEA);
    //ApproxAtmosphericRefraction(SEA, AAR);
    //SolarElevationCorrectedForATMRefraction(SEA, AAR, SECFATMR);
    //SolarAzimuthAngle(HA, SZA, ID.latitude, SD, SAA);



    //QVector<double>  resultVec;
    //for (const auto& vec : { TPL,  julian_days,  julian_century,
    //      GMLS,  GMAS,  EEO,  SEoC,  STL,  STA,  SRV,  SAL,  MOE,
    //      OC,  SRA,  SD,  VY,  EOT,  HAS,  SN,  SRT,  SST,  SLD,
    //      TST,  HA,  SZA,  SEA,  AAR,  SECFATMR,  SAA }) {
    //    resultVec.push_back(vec[0]);  // добавляем первый элемент в новый вектор
    //}
    //int k = 0;
}



bool SolarGeometryCalculatorNOAA::CheckValidationTime(QLineEdit* lineEdit, QLabel* statusLabel)
{
    QString dateTimeString = lineEdit->text();

    // 2. Преобразуем строку в QDateTime
    // Используем тот же формат, который был задан для ввода и валидации
    QString format = "yyyy-MM-dd HH:mm";
    lineEdit->setPlaceholderText(format); // Подсказка для пользователя
    QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

    // 3. Проверяем, успешно ли прошло преобразование
    if (dateTime.isValid()) {
        // Дата и время успешно распознаны
       // qDebug() << "Считанная дата и время:" << dateTime;

        // Теперь вы можете использовать объект dateTime для дальнейших операций:
        // int year = dateTime.date().year();
        // int month = dateTime.date().month();
        // int day = dateTime.date().day();
        // QTime time = dateTime.time();
        // ... и так далее
    }
    else {
        // Ошибка: строка не соответствует формату или содержит некорректную дату/время
        // Этого не должно произойти, если валидатор настроен правильно и включен,
        // но проверка все равно полезна.
       // qDebug() << "Ошибка: не удалось распознать дату и время из строки:" << dateTimeString;
        // Здесь можно предпринять действия по обработке ошибки,
        // например, показать сообщение пользователю.
        //lineEdit->setText("2025-01-01 00:00");
    }
    statusLabelsMap.insert(lineEdit, statusLabel);
    connect(lineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDateTimeInput);
    //QString input = dateTimeString; // Копируем для изменения
   // int pos = 0;
   // const QValidator* validator = lineEdit->validator();
    //if (validator) {
    //    QValidator::State state = validator->validate(input, pos);
    //    return state == QValidator::Acceptable;
    //}
    return dateTime.isValid();
}
void SolarGeometryCalculatorNOAA::validateDateTimeInput() {

    // Получаем указатель на QLineEdit, который отправил сигнал
    QLineEdit* editor = qobject_cast<QLineEdit*>(sender());
    if (!editor) {
        qDebug() << "sender() is not a QLineEdit!";
        return;
    }

    // Находим соответствующий QLabel для статуса
    QLabel* currentStatusLabel = statusLabelsMap.value(editor, nullptr);
    if (!currentStatusLabel) {
        qDebug() << "No status label found for editor:" << editor;
        // Можно создать временный QLabel или просто выводить в qDebug
        // Для примера, просто выйдем, если метка не найдена
        currentStatusLabel = new QLabel(); // Временная метка, чтобы не было падения
    }


    QString dateTimeString = editor->text();
    QString format = "yyyy-MM-dd HH:mm";
    QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

    // Проверяем состояние валидатора, если он есть
    bool isValidByValidator = true;
    const QValidator* validator = editor->validator();
    if (validator) {
        int pos = 0;
        isValidByValidator = (validator->validate(dateTimeString, pos) == QValidator::Acceptable);
    }


    if (dateTime.isValid() && isValidByValidator) {
        //currentStatusLabel->setText("Корректно: " + dateTime.toString(format));
        currentStatusLabel->setStyleSheet("color: green;");
        // qDebug() << "Проверка для" << editor->objectName() << "прошла: " << dateTime;
    }
    else if (editor->hasAcceptableInput()) { // Учитываем промежуточные состояния маски
      // Если валидатор QDateTimeValidator и маска, hasAcceptableInput может быть true для неполного ввода
      // Но fromString может вернуть невалидную дату.
      // QDateTimeValidator::Acceptable более строг.
        if (validator && validator->validate(dateTimeString, *(new int(0))) == QValidator::Intermediate) {
            //currentStatusLabel->setText("Ввод не завершен...");
            currentStatusLabel->setStyleSheet("color: orange;");
        }
        else if (!isValidByValidator && !dateTime.isValid()) { // Если и валидатор говорит Invalid и fromString
            //urrentStatusLabel->setText("Ошибка: неверный формат/дата.");
            currentStatusLabel->setStyleSheet("color: red;");
        }
        else { // Попытка обработать другие случаи
           // currentStatusLabel->setText("Формат не завершен или не точен.");
            currentStatusLabel->setStyleSheet("color: orange;");
        }
    }
    else {
       //currentStatusLabel->setText("Ошибка: неверный формат или некорректная дата/время.");
        currentStatusLabel->setStyleSheet("color: red;");
        // qDebug() << "Ошибка проверки для" << editor->objectName() << ":" << dateTimeString;
    }
}


 bool SolarGeometryCalculatorNOAA::CheckValidationLongitude()
{
        ui.le_longitude->setPlaceholderText("-180.0 : +180.0");
        QDoubleValidator* lonValidator = new QDoubleValidator(-180.0, 180.0, 7, ui.le_longitude);
        lonValidator->setNotation(QDoubleValidator::StandardNotation);
        ui.le_longitude->setValidator(lonValidator);
        statusLabelsMap.insert(ui.le_longitude, ui.l_longitude);
        connect(ui.le_longitude, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
        QString text = ui.le_longitude->text();
		int pos = 0;
	    return lonValidator->validate(text, pos) == QValidator::Acceptable;
    }
 bool SolarGeometryCalculatorNOAA::CheckValidationLatitude()
{
        ui.le_latitude->setPlaceholderText("-90.0 : +90.0");
        QDoubleValidator* latValidator = new QDoubleValidator(-90.0, 90.0, 7, ui.le_latitude); // 7 знаков после запятой
        latValidator->setNotation(QDoubleValidator::StandardNotation); // Обычная десятичная запись
        ui.le_latitude->setValidator(latValidator);
        statusLabelsMap.insert(ui.le_latitude, ui.l_latitude);
        connect(ui.le_latitude, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
        QString text = ui.le_latitude->text();
        int pos = 0;
        return latValidator->validate(text, pos) == QValidator::Acceptable;
    }


// bool SolarGeometryCalculatorNOAA::CheckValidationIntervalSec()
//{
//    ui.le_interval_time->setPlaceholderText(">= 0");
//    QIntValidator* intervalValidator = new QIntValidator(0, 2147483647, ui.le_interval_time); // От 0 до макс. int
//    ui.le_interval_time->setValidator(intervalValidator);
//    statusLabelsMap.insert(ui.le_interval_time, ui.l_interval_time);
//    connect(ui.le_interval_time, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
//    QString text = ui.le_interval_time->text();
//    int pos = 0;
//    return intervalValidator->validate(text, pos) == QValidator::Acceptable;
//}
 
void SolarGeometryCalculatorNOAA::validateDataSGCInput()
{
   
    // Получаем указатель на QLineEdit, который отправил сигнал
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (!edit) {
        qDebug() << "sender() is not a QLineEdit!";
        return;
    }

    // Находим соответствующий QLabel для статуса
    QLabel* statusLabel = statusLabelsMap.value(edit, nullptr);
    if (!statusLabel) {
        qDebug() << "No status label found for editor:" << edit;
        // Можно создать временный QLabel или просто выводить в qDebug
        // Для примера, просто выйдем, если метка не найдена
        statusLabel = new QLabel(); // Временная метка, чтобы не было падения
    }
    if (!edit->text().isEmpty() && !edit->hasAcceptableInput()) {
        // Ввод есть, но он не соответствует валидатору
    
        statusLabel->setStyleSheet("color: red;");
    }
    else if (edit->text().isEmpty() && edit->validator() != nullptr) {
        // Поле пустое, но если есть валидатор, можно считать это промежуточным состоянием
      
        statusLabel->setStyleSheet("color: orange;");
    }
    else if (edit->hasAcceptableInput() || edit->text().isEmpty()) {
        // Ввод корректен или поле пустое (и это допустимо)
   
        statusLabel->setStyleSheet("color: green;");
        if (edit->text().isEmpty()) { // Если пусто, вернуть исходный текст метки
            statusLabel->setStyleSheet("color: orange;");
       
        }
    }
}

void SolarGeometryCalculatorNOAA::ShowTableWidgetAmElev(const QVector<QVector<double>>& SGCData)
{

    if(!SGCData.isEmpty()){
        ui.tw_SGC->setRowCount(SGCData[0].size()+1);
        ui.tw_SGC->setColumnCount(SGCData.size());

        QVector<QString> columnNames;
        if (ui.cb_isAM->isChecked())
        {
            columnNames.push_back("AM");
        }

        if (ui.cB_isElevation->isChecked())
        {
            columnNames.push_back("Elev");
        }
        if (ui.cb_isAM_with_refr->isChecked())
        {
            columnNames.push_back("AM+Refr");
        }
        if (ui.cB_isElevation_with_refr->isChecked())
        {
            columnNames.push_back("Elev+Refr");
        }

        // ui.tw_SGC->setHorizontalHeaderLabels({ "Время (сек)", "Юлианская дата" });

        for (int i = 0; i < SGCData.size(); ++i) {
            ui.tw_SGC->setItem(0, i, new QTableWidgetItem(columnNames[i]));
            
            for (int j = 0; j < SGCData[i].size(); ++j) {
                ui.tw_SGC->setItem(j+1, i, new QTableWidgetItem(QString::number(SGCData[i][j], 'f', 2)));
                // table->setItem(i, 1, new QTableWidgetItem(QString::number(julianDays[i], 'f', 8)));
            }
        }
    }

}
void SolarGeometryCalculatorNOAA::CheckRowTableWidget(QMap<QString, int>& frequencyMap, QString name_col, const double min, const double max, const double precision)
{
    int rowCount = ui.tw_SGC->rowCount();
    int colCount = ui.tw_SGC->columnCount();

    for (int col = 0; col < colCount; ++col) {
        QTableWidgetItem* headerItem = ui.tw_SGC->item(0, col);
        if (!headerItem) continue;

        QString headerText = headerItem->text();
        if (!(headerText.startsWith(name_col)) )
            continue;  // Пропускаем столбцы без AM/AM Cor

        // Обрабатываем остальные строки (начиная со второй)
        for (int row = 1; row < rowCount; ++row) {
            QTableWidgetItem* item = ui.tw_SGC->item(row, col);
            if (!item) continue;

            bool ok = false;
            double value = item->text().toDouble(&ok);
            if (ok && value >= min && value <= max) {
                QString key = QString::number(value, 'f', precision);
                if (frequencyMap.contains(key))
                    frequencyMap[key]++;
            }
        }
        break;
    }
}
QVector<QMap<QString, int>> SolarGeometryCalculatorNOAA::GetAmStatistic()
{
    QVector<QMap<QString, int>> AMStat;
    if (!ui.tw_SGC->rowCount() == 0 && !ui.tw_SGC->columnCount() == 0)
    {
        const double minValue = 1.00;
        const double maxValue = 10.00;
        const double step = 0.01;
        const int precision = 2;

        // Подготовим карту частот
        QMap<QString, int> frequencyMapAm, frequencyMapAmCor;
        for (double val = minValue; val <= maxValue + 1e-6; val += step) {
            QString key = QString::number(val, 'f', precision);
            frequencyMapAm[key] = 0;
            frequencyMapAmCor[key] = 0;
        }
        CheckRowTableWidget(frequencyMapAm, "AM", minValue, maxValue, precision);
        AMStat.push_back(frequencyMapAm);
        if (ui.cb_isAM_with_refr->isChecked())
        {
            CheckRowTableWidget(frequencyMapAmCor, "AM+Refr", minValue, maxValue, precision);
            AMStat.push_back(frequencyMapAmCor);
        }
    }
    return AMStat;
}
void SolarGeometryCalculatorNOAA::ShowTableWidgetAmStatistic(const  QVector<QMap<QString, int>>& AMStat)
{
    if (AMStat.isEmpty())
        return;

    // Все QMap имеют одинаковые ключи
    const int rowCount = AMStat[0].size(); // количество уникальных AM значений
    const int colCount = AMStat.size();    // количество наборов статистики

    ui.tw_SGC_AM_statistic->clear();
    ui.tw_SGC_AM_statistic->setRowCount(rowCount);
    ui.tw_SGC_AM_statistic->setColumnCount(colCount + 1); // +1 — столбец для ключей (AM)

    // Заголовки столбцов
    QStringList headers;
    headers << "N";
    if (colCount > 0)
    {
        headers << QString("AM");
        if (colCount > 1)
        {
            headers << QString("AM+Cor");
        }
    }
        
    
    ui.tw_SGC_AM_statistic->setHorizontalHeaderLabels(headers);

    // Получим упорядоченные ключи
    QStringList keys = AMStat[0].keys();

    // Заполняем первую колонку (AM значения)
    for (int row = 0; row < rowCount; ++row) {
        ui.tw_SGC_AM_statistic->setItem(row, 0, new QTableWidgetItem(keys[row]));
    }

    // Заполняем значения статистики
    for (int col = 0; col < colCount; ++col) {
        const QMap<QString, int>& map = AMStat[col];
        for (int row = 0; row < rowCount; ++row) {
            QString key = keys[row];
            int value = map.value(key, 0); // безопасно, даже если нет ключа
            ui.tw_SGC_AM_statistic->setItem(row, col + 1, new QTableWidgetItem(QString::number(value)));
        }
    }

    ui.tw_SGC_AM_statistic->resizeColumnsToContents();
}

QString clean(const QString& s) {
    QString result = s;
    result.remove(QRegularExpression("[\\\\/:*?\"<>|= ]")); // удаляет недопустимые символы
    return result;
}


void SolarGeometryCalculatorNOAA::SaveSGC()
{
    SaveAMElevation(ui.tw_SGC, 0);
    SaveAMElevation(ui.tw_SGC_AM_statistic, 1);
}

void SolarGeometryCalculatorNOAA::SaveAMElevation(QTableWidget* table, int Var)
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    QMessageBox msgBox;

    if(Var == 0)
    {
        msgBox.setWindowTitle("Сохранение AM и Elevation");
    }
    if(Var == 1 ) {
        msgBox.setWindowTitle("Сохранение статистики по AM");
    }

    QString dirName = "./AM_Elevation_long_" + clean(ui.le_longitude->text()) + "_lat_" + clean(ui.le_latitude->text());
    QDir dir;
    //QMessageBox msg;
    //msg.setWindowTitle("Сохранение AM и Elevation");
    if (!dir.exists(dirName)) {
        if (!dir.mkpath(dirName)) {
            msgBox.setText(QString::fromUtf8("Не удалось создать папку:\n") + dirName);
            msgBox.exec();
            //QMessageBox::critical(this, QString::fromUtf8(u8"Ошибка"), );
            return;
        }
    }
    QString fileName;
    if (Var == 0)
    {
        fileName = "AM_Elevation_long_" + clean(ui.le_longitude->text())
            + "_lat_" + clean(ui.le_latitude->text())
            + "_time_" + clean(ui.le_UTC_Start_Date->text()) + "-" + clean(ui.le_UTC_End_Date->text()) + ".dat";

    }
    if (Var == 1) {
        fileName = "AM_Statistic_long_" + clean(ui.le_longitude->text())
            + "_lat_" + clean(ui.le_latitude->text())
            + "_time_" + clean(ui.le_UTC_Start_Date->text()) + "-" + clean(ui.le_UTC_End_Date->text()) + ".dat";

    }


    QString filePath = dirName + "/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        msgBox.setText("Не удалось открыть файл:\n" + filePath);
        msgBox.exec();
    	//QMessageBox::critical(this, QString::fromUtf8(u8"Ошибка"), "Не удалось открыть файл:\n" + filePath);
        return;
    }

    QTextStream out(&file);
    int rows = table->rowCount();
    int cols = table->columnCount();

    for (int i = 0; i < rows; ++i) {
        QStringList rowValues;
        for (int j = 0; j < cols; ++j) {
            QTableWidgetItem* item = table->item(i, j);
            rowValues << (item ? item->text() : "");
        }
        out << rowValues.join("\t") << "\n";
    }

    file.close();
    QString st = QString::fromUtf8(u8"Данные сохранены!");
    msgBox.setText(st);
    msgBox.exec();
}
