
#include "SGCNOAA.h"

#include <cmath>
#include <qvector.h>
#include <math.h>
#include <QDateTime>
#include <qmath.h>


double SinD(double angle) {
    return sin(angle / 180 * M_PI);
}
double CosD(double angle) {
    return cos(angle / 180 * M_PI);
}
double tanD(double angle) {
    return tan(angle / 180 * M_PI);
}
double toDegrees(double angle) {
    return angle * (180 / M_PI);
}
SGCNOAA::SGCNOAA(QDateTime startDateTime, QDateTime endDateTime, int lat, int lon, int interval)
    : startDateTime(startDateTime.isValid() ? startDateTime : QDateTime::fromString("2017-01-01 00:00", "yyyy-MM-dd HH:mm")),
    endDateTime(endDateTime.isValid() ? endDateTime : QDateTime::fromString("2017-01-01 00:00", "yyyy-MM-dd HH:mm")),
    latitude(lat),
    longitude(lon),
    interval(interval)
{
}
void SGCNOAA::TimePastLocal(QVector<double>& time) {
    QTime qtimeStart = startDateTime.time();
    QTime qtimeEnd = endDateTime.time();

    double time_ = qtimeStart.hour() *60 + qtimeStart.minute();
    //double minutesOfStartTime = (endDateTime.date().year() *  - startDateTime.date().year());
    //double minutesOfEndTime = (endDateTime.date().year() - startDateTime.date().year());
    //double delta_months = (endDateTime.date().year() - startDateTime.date().year());
    //double time_of_end =  qtimeStart.hour() * 60 * 60 + qtimeStart.minute() * 60;

    double secondsStarEnd = qtimeStart.secsTo(qtimeEnd) ;
    qDebug() << "Start DateTime:" << startDateTime;
    qDebug() << "Start Time:" << startDateTime.time();
    qDebug() << "End DateTime:" << endDateTime;
    qDebug() << "End Time:" << endDateTime.time();
    int finish_time = 1  * 60*24 / (10 * 24 * 6);
    time.clear();
    // Генерируем временные метки с шагом 1 секунда
    double interv = static_cast<double>(interval) / (10 * 24 * 60*6);
    //for (double t = time_; t <= finish_time; t += interv) {
    //for (double t = time_; t <= secondsStarEnd; t += interv) {
    //    time.push_back(t);
    //}
    double startTimeMin = startDateTime.toSecsSinceEpoch()/60;
    double endTimeMin = endDateTime.toSecsSinceEpoch() / 60;
    double startToEndTime = (endTimeMin - startTimeMin);
    for (double t = time_; t <= time_+startToEndTime; t += 1) {
        time.push_back(t - time_); // или просто t, в зависимости от задачи
    }
}
void SGCNOAA::JulianDay(QVector<double>& julian_days, Data& data, QVector<double>& TPL) {
    int  Y, M, A, B;
    float N;
    //Вводим Год, Месяц, Ltym.
    julian_days.clear();
    //Проверяем условие Month>2
    if (data.Month > 2) {
        Y = data.Year; M = data.Month;
    }
    else {
        Y = data.Year - 1; M = data.Month + 12;
    }
    //Вычисляем N по формуле.
    N = (data.Year + double(data.Month) / 100 + double(data.Day) / 10000);
    //Проверяем условие N>=1582.1015
    if (N >= 1582.1015) {
        A = int(Y / 100);  B = 2 - A + int(A / 4);
    }
    else {
        A = 0; B = 0;
    }
    // Конвертируем секунды в доли суток (1 секунда = 1/86400 дня)
    const double secondsToDays = 1.0 / 86400.0;

    //Вычисляем Юлианскую дату.
    //for (int i = 0; i < TPL.size(); i++) {
    //    julian_days.push_back(int(365.25 * Y) + int(30.6001 * (M + 1)) + data.Day + B + 1720994.5 + (TPL[i] / 60 / 24));
    //    // julian_days.push_back(
    //}
    // Вычисляем юлианскую дату для каждой временной метки
    for (double t : TPL) {
        double JD = int(365.25 * (Y + 4716))
            + int(30.6001 * (M + 1))
            + data.Day
            + B
            - 1524.5
            + (t * secondsToDays);
        julian_days.push_back(JD);
    }
}

void SGCNOAA::JulianCentury(QVector<double>& julian_century, QVector<double> & julian_days) {
    for (double JD : julian_days) {
        julian_century.push_back((JD - 2451545) / 36525);
    }

}

void SGCNOAA::GeomMeanLongSun(QVector<double>& julian_century, QVector<double>& GMLS)
{
    for (double JC : julian_century) {
        double _gmls = 280.46646 + JC * (36000.76983 + JC * 0.0003032);

        _gmls = (_gmls / 360 - trunc(_gmls / 360)) * 360;
        GMLS.push_back(_gmls);
    }
}

void SGCNOAA::GeomMeanAnomSun(QVector<double>& julian_century, QVector<double>& GMAS)
{
    for (double JC : julian_century) {
        GMAS.push_back(357.52911 + JC * (35999.05029 - 0.0001537 * JC));
    }
}

void SGCNOAA::EccentEarthOrbit(QVector<double>& julian_century, QVector<double>& EEO)
{
    for (double JC : julian_century) {
        EEO.push_back(0.016708634 - JC * (0.000042037 + 0.0000001267 * JC));
    }
}

void SGCNOAA::SunEqOfCtr(QVector<double>& julian_century, QVector<double>& GMAS, QVector<double>& SEoC)
{
    if (julian_century.size() == GMAS.size()) {
        for (int i = 0; i < julian_century.size(); i++) {
            SEoC.push_back(SinD(GMAS[i]) * (1.914602 - julian_century[i] * (0.004817 + 0.000014 * julian_century[i])) + SinD(2 * GMAS[i]) * (0.019993 - 0.000101 * julian_century[i]) + SinD(3 * GMAS[i]) * 0.000289);
        }
    }
}

void SGCNOAA::SunTrueLong(QVector<double>& GMLS, QVector<double>& SEoC, QVector<double>& STL)
{
    if (GMLS.size() == SEoC.size()) {
        for (int i = 0; i < GMLS.size(); i++) {
            STL.push_back(GMLS[i] + SEoC[i]);
        }
    }
}

void SGCNOAA::SunTrueAnom(QVector<double>& GMAS, QVector<double>& SEoC, QVector<double>& STA)
{
    if (GMAS.size() == SEoC.size()) {
        for (int i = 0; i < GMAS.size(); i++) {
            STA.push_back(GMAS[i] + SEoC[i]);
        }
    }
}

void SGCNOAA::SunRadVector(QVector<double>& EEO, QVector<double>& STA, QVector<double>& SRV)
{
    if (STA.size() == EEO.size()) {
        for (int i = 0; i < STA.size(); i++) {
            SRV.push_back((1.000001018 * (1 - EEO[i] * EEO[i])) / (1 + EEO[i] * CosD(STA[i])));
        }
    }
}

void SGCNOAA::SunAppLong(QVector<double>& julian_century, QVector<double>& STL, QVector<double>& SAL)
{
    if (STL.size() == julian_century.size()) {
        for (int i = 0; i < STL.size(); i++) {
            double sal = STL[i] - 0.00569 - 0.00478 * SinD(125.04 - 1934.136 * julian_century[i]);
            SAL.push_back(sal);
        }
    }
}

void SGCNOAA::MeanObligEcliptic(QVector<double> & julian_century, QVector<double>& MOE)
{
    for (double JD : julian_century) {
        MOE.push_back(23 + (26 + ((21.448 - JD * (46.815 + JD * (0.00059 - JD * 0.001813)))) / 60) / 60);
    }
}

void SGCNOAA::ObliqCorr(QVector<double>& julian_century, QVector<double>& MOE, QVector<double>& OC)
{
    if (MOE.size() == julian_century.size()) {
        for (int i = 0; i < MOE.size(); i++) {
            OC.push_back(MOE[i] + 0.00256 * CosD(125.04 - 1934.136 * julian_century[i]));
        }
    }
}

void SGCNOAA::SunRtAscen(QVector<double>& OC, QVector<double>& SAL, QVector<double>& SRA)
{
    if (OC.size() == SAL.size()) {
        for (int i = 0; i < OC.size(); i++) {
            double sra = toDegrees(atan(CosD(OC[i]) * SinD(SAL[i]) / CosD(SAL[i])));
            SRA.push_back(sra);
        }
    }
}

void SGCNOAA::SunDeclin(QVector<double>& SAL, QVector<double>& OC, QVector<double>& SD)
{
    if (OC.size() == SAL.size()) {
        for (int i = 0; i < OC.size(); i++) {
            double sd = toDegrees(asin(SinD(OC[i]) * SinD(SAL[i])));
            SD.push_back(sd);
        }
    }
}

void SGCNOAA::VarY(QVector<double>& OC, QVector<double>& VY)
{
    for (double oc : OC) {
        VY.push_back(tanD(oc / 2) * tanD(oc / 2));
    }
}

void SGCNOAA::EqOfTime(QVector<double>& GMLS, QVector<double>& GMAS, QVector<double>& EEO, QVector<double>& VY, QVector<double>& EOT)
{
    if (GMLS.size() == GMAS.size() && EEO.size() == VY.size() && EEO.size() == GMAS.size()) {
        for (int i = 0; i < VY.size(); i++) {
            double eot = 4 * toDegrees(VY[i] * SinD(2 * GMLS[i]) - 2 * EEO[i] * SinD(GMAS[i]) + 4 * EEO[i] * VY[i] * SinD(GMAS[i]) * CosD(2 * GMLS[i]) - 0.5 * VY[i] * VY[i] * SinD(4 * GMLS[i]) - 1.25 * EEO[i] * EEO[i] * SinD(2 * GMAS[i]));
            EOT.push_back(eot);
        }
    }
}

void SGCNOAA::HASunrise(int latitude, QVector<double>& SD, QVector<double>& HAS)
{
    for (double sd : SD) {
        double hasunrise = toDegrees(acos(CosD(90.833) / (CosD(latitude) * CosD(sd)) - tanD(latitude) * tanD(sd)));
        HAS.push_back(hasunrise);
    }
}

void SGCNOAA::SolarNoon(int longitude, int timezone, QVector<double>& EOT, QVector<double>& SN)
{
    for (double eot : EOT) {
        SN.push_back((720 - 4 * longitude - eot + timezone * 60) / 1440);
    }
}

void SGCNOAA::SunriseTime(QVector<double>& SN, QVector<double>& HAS, QVector<double>& SRT)
{
    if (SN.size() == HAS.size()) {
        for (int i = 0; i < SN.size(); i++) {
            SRT.push_back(SN[i] - HAS[i] * 4 / 1440);
        }
    }
}

void SGCNOAA::SunsetTime(QVector<double>& SN, QVector<double>& HAS, QVector<double>& SST)
{
    if (SN.size() == HAS.size()) {
        for (int i = 0; i < SN.size(); i++) {
            SST.push_back(SN[i] + HAS[i] * 4 / 1440);
        }
    }
}

void SGCNOAA::SunlightDuration(QVector<double>& HAS, QVector<double>& SLD)
{
    for (double has : HAS) {
        SLD.push_back(8 * has);
    }
}

void SGCNOAA::TrueSolarTime(QVector<double>& TPL, QVector<double>& EOT, int longitude, int timezone, QVector<double>& TST)
{
    if (TPL.size() == EOT.size()) {
        for (int i = 0; i < EOT.size(); i++) {
            double t = TPL[i] * 1440 + EOT[i] + 4 * longitude - 60 * timezone + 1440;
            double tst = (t / 1440 - trunc(t / 1440)) * 1440;
            //t = std::fmod(t, 1440);
            TST.push_back(tst);
        }
    }
}

void SGCNOAA::HourAngle(QVector<double>& TST, QVector<double>& HA)
{
    for (double tst : TST) {
        if (tst / 4 < 0) { HA.push_back(tst / 4 + 180); }
        else { HA.push_back(tst / 4 - 180); }
    }
}

void SGCNOAA::SolarZenitAngle(int latitude, QVector<double>& SD, QVector<double>& HA, QVector<double>& SZA)
{
    if (SD.size() == HA.size()) {
        for (int i = 0; i < SD.size(); i++) {
            double sza = toDegrees(acos(SinD(latitude) * SinD(SD[i]) + CosD(latitude) * CosD(SD[i]) * CosD(HA[i])));
            SZA.push_back(sza);
        }
    }
}

void SGCNOAA::SolarElevationAngle(QVector<double>& SZA, QVector<double>& SEA)
{
    for (double sza : SZA) {
        if ((90 - sza) >= 0)
        {
            SEA.push_back(90 - sza);
        }
        else SEA.push_back(0); 

    }
}

void SGCNOAA::AirMass(QVector<double>& SEA, QVector<double>& AM)
{
    const double degToRad = M_PI / 180.0;
    for (double sea : SEA)
    {
        //AM.push_back(1 / cos(SZA[i]));
        //AM.push_back(1 / (sin(SEA[i]) + 0.50572 * pow((6.07995 + SEA[i]), -1.6364)));
        if (sea < 0) { AM.push_back(0); }
        else
        {
            // Переводим угол из градусов в радианы
            double zenith_angle_rad = (90.0 - sea) * degToRad;
            double cos_zenith = cos(zenith_angle_rad);
            AM.push_back(1.0 / (cos_zenith + 0.50572 * pow(96.07995 - sea, -1.6364)));
        }

    }
}

void SGCNOAA::ApproxAtmosphericRefraction(QVector<double>& SEA, QVector<double>& AAR)
{
    for (double sea : SEA) {
        double aar = 0;
        if (sea > 85) {
            aar = 0;
        }
        else {
            if (sea > 5) {
                aar = 58.1 / tanD(sea) - 0.07 / pow(tanD(sea), 3) + 0.000086 / pow(tanD(sea), 5);

            }
            else {
                if (sea > -0.575) {
                    aar = 1735 + sea * (-518.2 + sea * (103.4 + sea * (-12.79 + sea * 0.711)));
                }
                else {
                    aar = -20.772 / tanD(sea);
                }
            }
        }
        aar = aar / 3600;
        AAR.push_back(aar);
    }
}

void SGCNOAA::SolarElevationCorrectedForATMRefraction(QVector<double>& SEA, QVector<double>& AAR, QVector<double>& SECFATMR)
{
    if (SEA.size() == AAR.size()) {
        for (int i = 0; i < SEA.size(); i++) {

            SECFATMR.push_back(SEA[i] + AAR[i]);
        }
    }
}

void SGCNOAA::AirMassCorrectedForATMRefraction(QVector<double>& SECFATMR, QVector<double>& AMCFATMR)
{
    const double degToRad = M_PI / 180.0;
    for (double sea : SECFATMR)
    {

        if (sea < 0) { AMCFATMR.push_back(-1); }
        else
        {
            // Переводим угол из градусов в радианы
            double zenith_angle_rad = (90.0 - sea) * degToRad;
            double cos_zenith = cos(zenith_angle_rad);
            AMCFATMR.push_back(1.0 / (cos_zenith + 0.50572 * pow(96.07995 - sea, -1.6364)));
        }

    }
}

void SGCNOAA::SolarAzimuthAngle(QVector<double>& HA, QVector<double>& SZA, int latitude, QVector<double>& SD, QVector<double>& SAA)
{

    if (HA.size() == SZA.size() && SZA.size() == SD.size()) {
        for (int i = 0; i < SZA.size(); i++) {
            double saa = 0;
            if (HA[i] > 0) {
                double tq = toDegrees(acos(((SinD(latitude) * CosD(SZA[i])) - SinD(SD[i])) / (CosD(latitude) * SinD(SZA[i])))) + 180;
                saa = std::fmod(tq, 360);
            }
            else {
                double tq = 540 - toDegrees(acos(((SinD(latitude) * CosD(SZA[i])) - SinD(SD[i])) / (CosD(latitude) * SinD(SZA[i]))));
                saa = std::fmod(tq, 360);

            }
            SAA.push_back(saa);
        }
    }
}




void SGCNOAA::getResult()
{
    QVector<double> TPL_, julian_days_, julian_century_, GMLS_, GMAS_, EEO_, SEoC_, STL_, STA_;
    QVector<double> SRV_, SAL_, MOE_, OC_, SRA_, SD_, VY_, EOT_, HAS_, SN_, SRT_, SST_, SLD_;
    QVector<double> TST_, HA_, SZA_,  AAR_,   SAA_;
	int timezone = 0;	QVector<double>* mass[30] = {
        &TPL_, &julian_days_, &julian_century_, &GMLS_, &GMAS_, &EEO_, &SEoC_, &STL_, &STA_, &SRV_,
        &SAL_, &MOE_, &OC_, &SRA_, &SD_, &VY_, &EOT_, &HAS_, &SN_, &SRT_, &SST_, &SLD_,
        &TST_, &HA_, &SZA_, &SEA_, &AM_, &AAR_, &SECFATMR_, &AMCFATMR_
    };

    Data data(startDateTime.date().day(), startDateTime.date().month(), startDateTime.date().year(),latitude,longitude,timezone);
	TimePastLocal(TPL_);
	JulianDay(julian_days_, data, TPL_);
	JulianCentury(julian_century_, julian_days_);
	GeomMeanLongSun(julian_century_, GMLS_);
	GeomMeanAnomSun(julian_century_, GMAS_);
	EccentEarthOrbit(julian_century_, EEO_);
	SunEqOfCtr(julian_century_, GMAS_, SEoC_);
	SunTrueLong(GMLS_, SEoC_, STL_);
	SunTrueAnom(GMAS_, SEoC_, STA_);
	SunRadVector(EEO_, STA_, SRV_);
	SunAppLong(julian_century_, STL_, SAL_);
	MeanObligEcliptic(julian_century_, MOE_);
	ObliqCorr(julian_century_, MOE_, OC_);
	SunRtAscen(OC_, SAL_, SRA_);
	SunDeclin(SAL_, OC_, SD_);
	VarY(OC_, VY_);
	EqOfTime(GMLS_, GMAS_, EEO_, VY_, EOT_);
	HASunrise(latitude, SD_, HAS_);
	SolarNoon(longitude, timezone, EOT_, SN_);
	SunriseTime(SN_, HAS_, SRT_);
	SunsetTime(SN_, HAS_, SST_);
	SunlightDuration(HAS_, SLD_);
	TrueSolarTime(TPL_, EOT_, longitude, timezone, TST_);
	HourAngle(TST_, HA_);
	SolarZenitAngle(latitude, SD_, HA_, SZA_);
	SolarElevationAngle(SZA_, SEA_);
    AirMass(SEA_, AM_);
	ApproxAtmosphericRefraction(SEA_, AAR_);
	SolarElevationCorrectedForATMRefraction(SEA_, AAR_, SECFATMR_);
    AirMassCorrectedForATMRefraction(SECFATMR_, AMCFATMR_);
	SolarAzimuthAngle(HA_, SZA_, latitude, SD_, SAA_);

}
