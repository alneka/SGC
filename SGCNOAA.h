#pragma once
#include <QDateTime>

class SGCNOAA
{
public:
	//  онструктор с параметрами по умолчанию
	explicit SGCNOAA(QDateTime startDateTime, QDateTime endDateTime, int lat = 0, int lon = 0, int interval = 1);

	const QVector<double>& getAM() const { return AM_; }
	const QVector<double>& getAMC() const { return AMCFATMR_; }
	const QVector<double>& getSEA() const { return SEA_; }
	const QVector<double>& getSECFATMR() const { return SECFATMR_; }

	void getResult();

private:
	QDateTime startDateTime, endDateTime;
	int latitude;
	int longitude;
	int interval;

	struct Data {
		int Day = 0;
		int Month = 0;
		int Year = 0;
		int Interval = 1;
		int latitude = 0;
		int longitude = 0;
		int timezone = 0;
	};


	QVector<double>  SEA_, AM_,SECFATMR_, AMCFATMR_;


	void TimePastLocal(QVector<double>& TPL);
	void JulianDay(QVector<double>& julian_days, Data& data, QVector<double>& TPL);
	void JulianCentury(QVector<double>& julian_century, QVector<double>& julian_days);
	void GeomMeanLongSun(QVector<double>& julian_century, QVector<double>& GMLS);
	void GeomMeanAnomSun(QVector<double>& julian_century, QVector<double>& GMAS);
	void EccentEarthOrbit(QVector<double>& julian_century, QVector<double>& EEO);
	void SunEqOfCtr(QVector<double>& julian_century,QVector<double>&GMAS, QVector<double>& SEoC);
	void SunTrueLong(QVector<double>& GMLS,QVector<double>&SEoC, QVector<double>& STL);
	void SunTrueAnom(QVector<double>& GMAS,QVector<double>&SEoC, QVector<double>& STA);
	void SunRadVector(QVector<double>& EEO,QVector<double>&STA, QVector<double>& SRV);
	void SunAppLong(QVector<double>& julian_century,QVector<double>&STA, QVector<double>& SAL);
	void MeanObligEcliptic(QVector<double>& julian_century, QVector<double>& MOE);
	void ObliqCorr(QVector<double>& julian_century,QVector<double>& MOE, QVector<double>& OC);
	void SunRtAscen(QVector<double>& OC,QVector<double>& SAL, QVector<double>& SRA);
	void SunDeclin(QVector<double>& SAL,QVector<double>& OC, QVector<double>& SD);
	void VarY(QVector<double>& OC, QVector<double>& VY);
	void EqOfTime(QVector<double>& GMLS, QVector<double>& GMAS, QVector<double>& EEO, QVector<double>& VY, QVector<double>& EOT);
	void HASunrise(int latitude,QVector<double>& SD, QVector<double>& HAS);
	void SolarNoon(int longitude, int timezone,QVector<double>& EOT, QVector<double>& SN);
	void SunriseTime(QVector<double>& SN,QVector<double>& HAS, QVector<double>& SRT);
	void SunsetTime(QVector<double>& SN,QVector<double>& HAS, QVector<double>& SST);
	void SunlightDuration(QVector<double>& HAS, QVector<double>& SLD);
	void TrueSolarTime(QVector<double>& TPL,QVector<double>& EOT, int longitude, int timezone, QVector<double>& TST);
	void HourAngle(QVector<double>& TST, QVector<double>& HA);
	void SolarZenitAngle(int latitude,QVector<double>& SD,QVector<double>& HA, QVector<double>& SZA);
	void SolarElevationAngle(QVector<double>& SZA, QVector<double>& SEA);
	void AirMass(QVector<double>& SEA, QVector<double>& AM);
	void ApproxAtmosphericRefraction(QVector<double>& SEA, QVector<double>& AAR);
	void SolarElevationCorrectedForATMRefraction(QVector<double>& SEA,QVector<double>& AAR, QVector<double>& SECFATMR);
	void AirMassCorrectedForATMRefraction(QVector<double>& SECFATMR, QVector<double>& AMCFATMR);
	void SolarAzimuthAngle(QVector<double>& HA,QVector<double>& SZA, int latitude,QVector<double>& SD, QVector<double>& SAA);


};

