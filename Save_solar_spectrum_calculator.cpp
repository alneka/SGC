#define _CRT_SECURE_NO_WARNINGS
#include "Solar_spectrum_calculator.h"
#include <QtCore/qdir.h>


void Solar_spectrum_calculator::SaveSpec(double BetaPar, double AMpar, QVector<double> Spectr, double* AM0_WL)
{

	QDir dir(".//Spectrum/");
	QString dirName = QString("M") + QString::number(BetaPar);
	if (!dir.exists(dirName)) {
		dir.mkpath(dirName);
	}

	std::string sbp = (QString::number(BetaPar)).toStdString();
	std::string sam = QString::number(AMpar).toStdString();
	//std::string path =( ".//M" + std::to_string(BetaPar) + "/"+"M" + std::to_string(BetaPar) + "AM" + std::to_string(AMpar) + ".dat");
	std::string path = (".//Spectrum/M" + sbp + "/" + "M" + sbp + "AM" + sam + ".dat");
	FILE* f = fopen(path.c_str(), "wt");
	for (int i = 0; i < Spectr.size(); i++) {
		fprintf(f, "%10g	%10g	\n", AM0_WL[i] * 1000, Spectr[i]);
		//power += Spectr[i];
	}
	fclose(f);

}

void Solar_spectrum_calculator::SavePowerAM(double BetaPar, QVector<double> v_am, QVector<double> v_power)
{
	QDir dir(".//Spectrum/");
	QString dirName = QString("M") + QString::number(BetaPar);
	if (!dir.exists(dirName)) {
		dir.mkpath(dirName);
	}
	std::string sbp = (QString::number(BetaPar)).toStdString();


	std::string path = (".//Spectrum/M" + sbp + "/" + "M" + sbp + "power" + ".dat");
	FILE* f = fopen(path.c_str(), "wt");
	for (int i = 0; i < v_am.size(); i++) {
		fprintf(f, "%10g	%10g	\n", v_am[i], v_power[i]);
	}
	fclose(f);
}
