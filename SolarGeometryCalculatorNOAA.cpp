
#include "SolarGeometryCalculatorNOAA.h"

#include "SGCNOAA.h"
#include <QRegularExpressionValidator>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


SolarGeometryCalculatorNOAA::SolarGeometryCalculatorNOAA(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //QRegularExpression rx("^([0-9]{4}) (0[1-9]|1[0-2]) (0[1-9]|[12][0-9]|3[01]) (0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$");
    //QRegularExpressionValidator* regexValidator = new QRegularExpressionValidator(rx, this); // 'this' ������ ���� QObject*
    CheckValidationTime(ui.le_UTC_Start_Date, ui.l_UTC_Start_Date);
    CheckValidationTime(ui.le_UTC_End_Date, ui.l_UTC_End_Date);
    CheckValidationLongitude();
    CheckValidationLatitude();
    CheckValidationIntervalSec();
    // ��������������, ��� ui.le_UTC_Start_Date - ��� QLineEdit*
    //ui.le_UTC_Start_Date->setValidator(regexValidator);
    //ui.le_UTC_End_Date->setValidator(regexValidator);

    connect(ui.pb_SGC_start, SIGNAL(clicked()), this, SLOT(InputData()));
    connect(ui.pb_SGC_AM_Elev_save, SIGNAL(clicked()), this, SLOT(SaveAMElevation()));
    //connect(ui.le_UTC_Start_Date, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::CheckValidationTime(*this));
}

SolarGeometryCalculatorNOAA::~SolarGeometryCalculatorNOAA()
{}

void SolarGeometryCalculatorNOAA::InputData() {

    QVector<bool> vec_validate = { CheckValidationTime(ui.le_UTC_Start_Date, ui.l_UTC_Start_Date),
    	CheckValidationTime(ui.le_UTC_End_Date, ui.l_UTC_End_Date),
		CheckValidationLongitude(),
		CheckValidationLatitude(),
		CheckValidationIntervalSec() };

   if(vec_validate.contains(true))
    {
        // 1. �������� ������ ���� �� ����������������� ����������
        QString dateTimeString = ui.le_UTC_Start_Date->text();

        // 2. ���������� ������ ������ ���� � ����������� � � QDateTime
        QString format = "yyyy-MM-dd HH:mm";
        QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

        // 3. ��������� ������, ������� � �������� �������
        int longitude = ui.le_longitude->text().toInt();
        int latitude = ui.le_latitude->text().toInt();
        int interval = ui.le_interval_time->text().toInt();
        interval = 60; // 1 min 
        // 4. ������ ������ SGCNOAA � ��������� �����������
        SGCNOAA sgcnoaa(dateTime, latitude, longitude, interval);

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

        //SGC.push_back(vec_AM);
        ShowTableWidget(SGC);
 /*       QStandardItemModel* model = new QStandardItemModel();

        for (double value : vec_AM) {
            QStandardItem* item = new QStandardItem(QString::number(value, 'f', 2));
            model->appendRow(item);
        }

        ui.listView->setModel(model);*/
    };

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
    //    resultVec.push_back(vec[0]);  // ��������� ������ ������� � ����� ������
    //}
    //int k = 0;
}



bool SolarGeometryCalculatorNOAA::CheckValidationTime(QLineEdit* lineEdit, QLabel* statusLabel)
{
    QString dateTimeString = lineEdit->text();

    // 2. ����������� ������ � QDateTime
    // ���������� ��� �� ������, ������� ��� ����� ��� ����� � ���������
    QString format = "yyyy-MM-dd HH:mm";
    lineEdit->setPlaceholderText(format); // ��������� ��� ������������
    QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

    // 3. ���������, ������� �� ������ ��������������
    if (dateTime.isValid()) {
        // ���� � ����� ������� ����������
       // qDebug() << "��������� ���� � �����:" << dateTime;

        // ������ �� ������ ������������ ������ dateTime ��� ���������� ��������:
        // int year = dateTime.date().year();
        // int month = dateTime.date().month();
        // int day = dateTime.date().day();
        // QTime time = dateTime.time();
        // ... � ��� �����
    }
    else {
        // ������: ������ �� ������������� ������� ��� �������� ������������ ����/�����
        // ����� �� ������ ���������, ���� ��������� �������� ��������� � �������,
        // �� �������� ��� ����� �������.
       // qDebug() << "������: �� ������� ���������� ���� � ����� �� ������:" << dateTimeString;
        // ����� ����� ����������� �������� �� ��������� ������,
        // ��������, �������� ��������� ������������.
        //lineEdit->setText("2025-01-01 00:00");
    }
    statusLabelsMap.insert(lineEdit, statusLabel);
    connect(lineEdit, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDateTimeInput);
    //QString input = dateTimeString; // �������� ��� ���������
   // int pos = 0;
   // const QValidator* validator = lineEdit->validator();
    //if (validator) {
    //    QValidator::State state = validator->validate(input, pos);
    //    return state == QValidator::Acceptable;
    //}
    return dateTime.isValid();
}
void SolarGeometryCalculatorNOAA::validateDateTimeInput() {

    // �������� ��������� �� QLineEdit, ������� �������� ������
    QLineEdit* editor = qobject_cast<QLineEdit*>(sender());
    if (!editor) {
        qDebug() << "sender() is not a QLineEdit!";
        return;
    }

    // ������� ��������������� QLabel ��� �������
    QLabel* currentStatusLabel = statusLabelsMap.value(editor, nullptr);
    if (!currentStatusLabel) {
        qDebug() << "No status label found for editor:" << editor;
        // ����� ������� ��������� QLabel ��� ������ �������� � qDebug
        // ��� �������, ������ ������, ���� ����� �� �������
        currentStatusLabel = new QLabel(); // ��������� �����, ����� �� ���� �������
    }


    QString dateTimeString = editor->text();
    QString format = "yyyy-MM-dd HH:mm";
    QDateTime dateTime = QDateTime::fromString(dateTimeString, format);

    // ��������� ��������� ����������, ���� �� ����
    bool isValidByValidator = true;
    const QValidator* validator = editor->validator();
    if (validator) {
        int pos = 0;
        isValidByValidator = (validator->validate(dateTimeString, pos) == QValidator::Acceptable);
    }


    if (dateTime.isValid() && isValidByValidator) {
        //currentStatusLabel->setText("���������: " + dateTime.toString(format));
        currentStatusLabel->setStyleSheet("color: green;");
        // qDebug() << "�������� ���" << editor->objectName() << "������: " << dateTime;
    }
    else if (editor->hasAcceptableInput()) { // ��������� ������������� ��������� �����
      // ���� ��������� QDateTimeValidator � �����, hasAcceptableInput ����� ���� true ��� ��������� �����
      // �� fromString ����� ������� ���������� ����.
      // QDateTimeValidator::Acceptable ����� �����.
        if (validator && validator->validate(dateTimeString, *(new int(0))) == QValidator::Intermediate) {
            //currentStatusLabel->setText("���� �� ��������...");
            currentStatusLabel->setStyleSheet("color: orange;");
        }
        else if (!isValidByValidator && !dateTime.isValid()) { // ���� � ��������� ������� Invalid � fromString
            //urrentStatusLabel->setText("������: �������� ������/����.");
            currentStatusLabel->setStyleSheet("color: red;");
        }
        else { // ������� ���������� ������ ������
           // currentStatusLabel->setText("������ �� �������� ��� �� �����.");
            currentStatusLabel->setStyleSheet("color: orange;");
        }
    }
    else {
       //currentStatusLabel->setText("������: �������� ������ ��� ������������ ����/�����.");
        currentStatusLabel->setStyleSheet("color: red;");
        // qDebug() << "������ �������� ���" << editor->objectName() << ":" << dateTimeString;
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
        QDoubleValidator* latValidator = new QDoubleValidator(-90.0, 90.0, 7, ui.le_latitude); // 7 ������ ����� �������
        latValidator->setNotation(QDoubleValidator::StandardNotation); // ������� ���������� ������
        ui.le_latitude->setValidator(latValidator);
        statusLabelsMap.insert(ui.le_latitude, ui.l_latitude);
        connect(ui.le_latitude, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
        QString text = ui.le_latitude->text();
        int pos = 0;
        return latValidator->validate(text, pos) == QValidator::Acceptable;
    }


 bool SolarGeometryCalculatorNOAA::CheckValidationIntervalSec()
{
    ui.le_interval_time->setPlaceholderText(">= 0");
    QIntValidator* intervalValidator = new QIntValidator(0, 2147483647, ui.le_interval_time); // �� 0 �� ����. int
    ui.le_interval_time->setValidator(intervalValidator);
    statusLabelsMap.insert(ui.le_interval_time, ui.l_interval_time);
    connect(ui.le_interval_time, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
    QString text = ui.le_interval_time->text();
    int pos = 0;
    return intervalValidator->validate(text, pos) == QValidator::Acceptable;
}
 
void SolarGeometryCalculatorNOAA::validateDataSGCInput()
{
    // �������� ��������� �� QLineEdit, ������� �������� ������
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (!edit) {
        qDebug() << "sender() is not a QLineEdit!";
        return;
    }

    // ������� ��������������� QLabel ��� �������
    QLabel* statusLabel = statusLabelsMap.value(edit, nullptr);
    if (!statusLabel) {
        qDebug() << "No status label found for editor:" << edit;
        // ����� ������� ��������� QLabel ��� ������ �������� � qDebug
        // ��� �������, ������ ������, ���� ����� �� �������
        statusLabel = new QLabel(); // ��������� �����, ����� �� ���� �������
    }
    if (!edit->text().isEmpty() && !edit->hasAcceptableInput()) {
        // ���� ����, �� �� �� ������������� ����������
    
        statusLabel->setStyleSheet("color: red;");
    }
    else if (edit->text().isEmpty() && edit->validator() != nullptr) {
        // ���� ������, �� ���� ���� ���������, ����� ������� ��� ������������� ����������
      
        statusLabel->setStyleSheet("color: orange;");
    }
    else if (edit->hasAcceptableInput() || edit->text().isEmpty()) {
        // ���� ��������� ��� ���� ������ (� ��� ���������)
   
        statusLabel->setStyleSheet("color: green;");
        if (edit->text().isEmpty()) { // ���� �����, ������� �������� ����� �����
            statusLabel->setStyleSheet("color: orange;");
       
        }
    }
}

void SolarGeometryCalculatorNOAA::ShowTableWidget(const QVector<QVector<double>> SGCData)
{
    if(!SGCData.isEmpty()){
        ui.tw_SGC->setRowCount(SGCData[0].size());
        ui.tw_SGC->setColumnCount(SGCData.size());
        // ui.tw_SGC->setHorizontalHeaderLabels({ "����� (���)", "��������� ����" });

        for (int i = 0; i < SGCData.size(); ++i) {
            for (int j = 0; j < SGCData[i].size(); ++j) {
                ui.tw_SGC->setItem(j, i, new QTableWidgetItem(QString::number(SGCData[i][j])));
                // table->setItem(i, 1, new QTableWidgetItem(QString::number(julianDays[i], 'f', 8)));
            }
        }
    }

}

void SolarGeometryCalculatorNOAA::SaveAMElevation()
{
    const QString& dirName = "AM(Elevation) on long = " + ui.le_longitude->text() + " lat =" + ui.le_latitude->text();
    QDir dir;
    if (!dir.exists(dirName)) {
        if (!dir.mkpath(dirName)) {
            //QMessageBox::critical(nullptr, QStringLiteral("������"), QStringLiteral("�� ������� ������� ����� ") + dirName);
            return;
        }
    }
    const QString& fileName = "AM(Elevation)_long=" + ui.le_longitude->text() + "_lat =" + ui.le_latitude->text()
							+ "_time = " + ui.le_UTC_Start_Date->text() + "-" + ui.le_UTC_End_Date->text()+".dat";
    QString filePath = dirName + "/" + fileName;
    QFile file(filePath);
    
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            //QMessageBox::critical(nullptr, QStringLiteral("������"), QStringLiteral("�� ������� ������� ����: ") + filePath);
            return;
        }
    
    QTextStream out(&file);
    int rows = ui.tw_SGC->rowCount();
    int cols = ui.tw_SGC->columnCount();

    for (int i = 0; i < rows; ++i) {
        QStringList rowValues;
        for (int j = 0; j < cols; ++j) {
            QTableWidgetItem* item = ui.tw_SGC->item(i, j);
            rowValues << (item ? item->text() : "");
        }
        out << rowValues.join("\t") << "\n";
    }

    file.close();
    //QMessageBox::information(nullptr, QStringLiteral("����������"), QStringLiteral("������ ��������� �: ") + filePath);
}
