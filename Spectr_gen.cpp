#define _CRT_SECURE_NO_WARNINGS
#include "Spectr_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QDir>
#include <cmath>
#include <qregularexpression.h>
#include <qstring.h>
#include <QtConcurrent>
#include "Solar_spectrum_calculator.h"

const int AM0_SIZE_ARR = 10000;
double AM0_WL[AM0_SIZE_ARR];
double AM0_P[AM0_SIZE_ARR];
double ALPHA_WL[AM0_SIZE_ARR];
double ALPHA_OL[AM0_SIZE_ARR];
double ALPHA_UL[AM0_SIZE_ARR];

int SIZE_OF_SPECTRUM = 0;




void Solar_spectrum_calculator::GetSpectAM0andAlpha(QString spec)
{
	FILE* fr;

	std::string Spec1 = spec.toStdString();
	const char* filename = Spec1.c_str();
	//char filename[256];

	//sprintf(filename, ".//Data/AM0.dat");
	fr = fopen(filename, "rt");

	char buf[80];
	while (!feof(fr)) { fgets(buf, 4000, fr); ++SIZE_OF_SPECTRUM; }
	rewind(fr);


	for (int num = 0; num < SIZE_OF_SPECTRUM; num++) {
		if (feof(fr)) break;
		double wl = 0;
		double power = 0;
		fscanf(fr, "%lf %lf", &wl, &power);
		AM0_WL[num] = wl;
		AM0_P[num] = power;
	}

	fclose(fr);


	char filename1[256];

	sprintf(filename1, ".//Data/ALPHA2.dat");
	fr = fopen(filename1, "rt");



	for (int num = 0; num < SIZE_OF_SPECTRUM; num++) {
		if (feof(fr)) break;
		double wl = 0;
		double ol = 0;
		double ul = 0;
		//fscanf(fr, "%lf %lf %lf", &wl, &ol, &ul);

		char line[128];
		//fscanf(fr, "%lf %lf", &spectrum[i].wl, &spectrum[i].en);
		fgets(line, sizeof line, fr);
		sscanf(line, "%lf %lf %lf", &wl, &ol, &ul);
		ALPHA_WL[num] = wl;
		ALPHA_OL[num] = ol;
		ALPHA_UL[num] = ul;
	}

	fclose(fr);
}


double Solar_spectrum_calculator::AM_d(double AMpar, double Press)
{
	return AMpar * Press / 1013.25;
}

void Solar_spectrum_calculator::KR_v(double AM, QVector<double>& Kr)
{
	for (int i = 0; i < SIZE_OF_SPECTRUM; i++) {
		double d_kr = exp(-AM / (pow(AM0_WL[i], 4) * (115.6406 - 1.335 / pow(AM0_WL[i], 2))));
		Kr.push_back(d_kr);
	}
}

void Solar_spectrum_calculator::Beta_v(double BetaPar, QVector<double>& Beta)
{
	std::for_each(AM0_WL, AM0_WL + SIZE_OF_SPECTRUM, [&](double x) { Beta.push_back(3.65868 * BetaPar * exp(-x * 1000 / 224.27483) + 0.8712 * BetaPar * exp(-x * 1000 / 997.90428) + 0.06998 * BetaPar); });
}

void Solar_spectrum_calculator::Ka_v(QVector<double> Beta, double AM, QVector<double>& Ka)
{
	//std::transform(Beta.begin(), Beta.end(), Ka.begin(),
	//	[AM](int x) { return exp(-x * AM); });

	std::for_each(Beta.begin(), Beta.end(), [&](double x) { Ka.push_back(exp(-x * AM)); });
}

void Solar_spectrum_calculator::Kw_v(double W, double AM, QVector<double>& Kw)
{
	std::for_each(ALPHA_WL, ALPHA_WL + SIZE_OF_SPECTRUM, [&](double x) {
		Kw.push_back(exp(-0.2385 * x * W * AM / (pow(1 + 20.07 * x * W * AM, 0.45)))); });
}

void Solar_spectrum_calculator::Ko_v(double OZ, double AM, QVector<double>& Ko)
{
	std::for_each(ALPHA_OL, ALPHA_OL + SIZE_OF_SPECTRUM, [&](double x) {
		Ko.push_back(exp(-OZ * AM * x)); });
}

void Solar_spectrum_calculator::Ku_v(double AM, QVector<double>& Ku)
{
	std::for_each(ALPHA_UL, ALPHA_UL + SIZE_OF_SPECTRUM, [&](double x) {
		Ku.push_back(exp(-1.41 * x * AM / (pow(1 + 118.93 * x * AM, 0.45)))); });
}

void Solar_spectrum_calculator::Spectrum(double D, QVector<double> Kr, QVector<double> Ka, QVector<double> Kw, QVector<double> Ko, QVector<double> Ku, QVector<double>& Spectr, double& power)
{
	power = 0;
	for (int i = 0; i < Kr.size(); i++) {
		double sp = AM0_P[i] * D * Kr[i] * Ka[i] * Kw[i] * Ko[i] * Ku[i];
		Spectr.push_back(sp);
		power += sp;

	}
}

void Solar_spectrum_calculator::Set_AM_Power_beta(double BetaPar, double AMpar_first, double AMpar_last, double step, double D, double Press, double W, double OZ, QString pathSP)
{
	//QDir dir(pathSP);
	//QString dirName = QString("M") + QString::number(BetaPar);
	//if (!dir.exists(dirName)) {
	//	dir.mkpath(dirName);
	//}
	//else {
	//	QDir subDir(dir.filePath(dirName));

	//	// Удаляем всё содержимое папки рекурсивно
	//	bool success = subDir.removeRecursively();
	//	if (!success) {
	//		qDebug() << "❌ Не удалось удалить содержимое папки:" << subDir.absolutePath();
	//	}
	//	else {
	//		qDebug() << "✅ Содержимое папки удалено:" << subDir.absolutePath();
	//	}

	//	// После этого создаем пустую папку снова
	//	dir.mkpath(dirName);
	//}
	//QVector<double>  v_power;
	//std::vector<std::string> v_am;

	//for (double AMpar = AMpar_first; AMpar <= AMpar_last; AMpar += step) {
	//	double power = 0;
	//	QVector<double> Kr;
	//	QVector<double> Beta;
	//	QVector<double> Ka;
	//	QVector<double> Kw;
	//	QVector<double> Ko;
	//	QVector<double> Ku;
	//	QVector<double> Spectr;
	//	//v_am.push_back(QString::number(AMpar, 'f', 2).remove(QRegularExpression("\\.$")));
	//	double AM = AM_d(AMpar, Press);
	//	KR_v(AM, Kr);
	//	Beta_v(BetaPar, Beta);
	//	Ka_v(Beta, AM, Ka);
	//	Kw_v(W, AM, Kw);
	//	Ko_v(OZ, AM, Ko);
	//	Ku_v(AM, Ku);
	//	Spectrum(D, Kr, Ka, Kw, Ko, Ku, Spectr, power);

	//	//double power = std::accumulate(Spectr.begin(), Spectr.end(), 0);
	//	v_power.push_back(power);

	//	std::string sbp = (QString::number(BetaPar)).toStdString();
	//	std::string sam = QString::number(AMpar, 'f', 2).remove(QRegularExpression("\\.$")).toStdString();
	//	//std::string path =( ".//M" + std::to_string(BetaPar) + "/"+"M" + std::to_string(BetaPar) + "AM" + std::to_string(AMpar) + ".dat");
	//	//std::string pathAM = (".//Spectrum/M" + sbp + "/" + "M" + sbp + "AM" + sam + ".dat");
	//	std::string pathT = "M" + sbp + "/";
	//	v_am.push_back(sam);
	//	//QString qpathAM = (pathSP + pathT + "M" + sbp + "AM" + sam + ".dat");
	//	std::string pathAM = (pathSP.toStdString() + pathT + "M" + sbp + "AM" + sam + ".dat");
	//	FILE* f = fopen(pathAM.c_str(), "wt");
	//	for (int i = 0; i < Spectr.size(); i++) {
	//		fprintf(f, "%10g	%10g	\n", AM0_WL[i] * 1000, Spectr[i]);
	//		//power += Spectr[i];
	//	}
	//	fclose(f);


	//}
	//
	//std::string foldpath = (pathSP + "M" + QString::number(BetaPar)).toStdString();
	//std::string sbp = (QString::number(BetaPar)).toStdString();


	//std::string pathPower = (foldpath + "/" + "M" + sbp + "power" + ".dat");
	//FILE* f = fopen(pathPower.c_str(), "wt");
	//for (int i = 0; i < v_am.size(); i++) {
	//	fprintf(f, "%s	%10g	\n", v_am[i].c_str(), v_power[i]);
	//}
	//fclose(f);
	
		QDir dir(pathSP);
		QString dirName = QString("M") + QString::number(BetaPar);
		if (!dir.exists(dirName)) {
			dir.mkpath(dirName);
		}
		else {
			QDir subDir(dir.filePath(dirName));
			bool success = subDir.removeRecursively();
			if (!success) {
				qDebug() << "❌ Не удалось удалить содержимое папки:" << subDir.absolutePath();
			}
			else {
				qDebug() << "✅ Содержимое папки удалено:" << subDir.absolutePath();
			}
			dir.mkpath(dirName);
		}

		QVector<double> v_power;               // Итоговые мощности
		QVector<QString> v_am;                 // Значения AM
		QMutex resultMutex;                    // Для синхронизации записи итогового списка

		// Список всех AM для распараллеливания
		QVector<double> amValues;
		for (double AMpar = AMpar_first; AMpar <= AMpar_last; AMpar += step) {
			amValues.append(AMpar);
		}

		// Функция для обработки одного AM
		auto processAM = [&](double AMpar) {
			double power = 0.0;

			QVector<double> Kr, Beta, Ka, Kw, Ko, Ku, Spectr;

			double AM = AM_d(AMpar, Press);
			KR_v(AM, Kr);
			Beta_v(BetaPar, Beta);
			Ka_v(Beta, AM, Ka);
			Kw_v(W, AM, Kw);
			Ko_v(OZ, AM, Ko);
			Ku_v(AM, Ku);
			Spectrum(D, Kr, Ka, Kw, Ko, Ku, Spectr, power);

			// Записываем спектр в файл
			QString fileName = QString("M%1AM%2.dat")
				.arg(BetaPar)
				.arg(AMpar, 0, 'f', 2)
				.replace(QRegularExpression("\\.$"), "");
			QString filePath = dir.filePath(dirName + "/" + fileName);
			QFile file(filePath);
			if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&file);
				out.setRealNumberPrecision(6);
				for (int i = 0; i < Spectr.size(); ++i) {
					out << AM0_WL[i] * 1000 << "\t" << Spectr[i] << "\n";
				}
				file.close();
			}

			// Сохраняем результат мощности и AM (потокобезопасно)
			resultMutex.lock();
			v_power.append(power);
			v_am.append(QString::number(AMpar, 'f', 2).remove(QRegularExpression("\\.$")));
			resultMutex.unlock();
		};

		// Распараллеливаем обработку всех AM
		QtConcurrent::blockingMap(amValues, processAM);

		// Записываем файл мощности
		QString powerFilePath = dir.filePath(dirName + QString("/M%1power.dat").arg(BetaPar));
		QFile powerFile(powerFilePath);
		if (powerFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&powerFile);
			for (int i = 0; i < v_am.size(); ++i) {
				out << v_am[i] << "\t" << v_power[i] << "\n";
			}
			powerFile.close();
		}
	
}
