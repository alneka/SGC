#pragma once

#include "qvector.h"




void GetSpectAM0andAlpha(QString BasicAM);

double AM_d(double AMpar, double Press);
void KR_v(double AM, QVector<double>& Kr);
void Beta_v(double BetaPar, QVector<double>& Beta);
void Ka_v(QVector<double>Beta, double AM, QVector<double>& Ka);
void Kw_v(double W, double AM, QVector<double>& Kw);
void Ko_v(double OZ, double AM, QVector<double>& Ko);
void Ku_v(double AM, QVector<double>& Ku);
void Spectrum(double D, QVector<double> Kr, QVector<double> Ka, QVector<double> Kw, QVector<double> Ko, QVector<double> Ku, QVector<double>& Spectr, double& power);
void Set_AM_Power_beta(double BetaPar, double AMpar_first, double AMpar_last, double step, double D, double Press, double W, double OZ, QString Path);
