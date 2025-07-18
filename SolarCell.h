#pragma once

#include "PNTransition.h"
#include "SolarGeometryCalculatorNOAA.h"
#include "Solar_spectrum_calculator.h"

class SolarCell : public QObject
{
	Q_OBJECT
public:

	SolarCell(SolarCellPowerLogic* logic, SolarGeometryCalculatorNOAA* sgcnoaaWindow, Solar_spectrum_calculator* solSpecCalc) : logic(logic), sgcnoaaWindow(sgcnoaaWindow), solSpecCalc(solSpecCalc)
	{
		CheckValidationCellSize();
		CheckValidationUoc();
		CheckValidationFF();
		//connect(ui.spravka, SIGNAL(triggered()), this, SLOT(info()));
		connect(sgcnoaaWindow->ui.pb_input_sc_data, SIGNAL(clicked()), this, SLOT(GetNumPN()));
		connect(sgcnoaaWindow->ui.pb_1_pn, SIGNAL(clicked()), this, SLOT(input0()));
		connect(sgcnoaaWindow->ui.pb_2_pn, SIGNAL(clicked()), this, SLOT(input1()));
		connect(sgcnoaaWindow->ui.pb_3_pn, SIGNAL(clicked()), this, SLOT(input2()));
		connect(sgcnoaaWindow->ui.pb_4_pn, SIGNAL(clicked()), this, SLOT(input3()));
		connect(sgcnoaaWindow->ui.pb_5_pn, SIGNAL(clicked()), this, SLOT(input4()));
		connect(sgcnoaaWindow->ui.pb_6_pn, SIGNAL(clicked()), this, SLOT(input5()));
		connect(sgcnoaaWindow->ui.pb_show_PowerCellGraphs, &QPushButton::clicked,
			this, &SolarCell::showPowerCellGraphs);
		connect(sgcnoaaWindow->ui.pb_GetPowerAMPN, SIGNAL(clicked()), this, SLOT(GetPowerPNrange()));


	};
	~SolarCell();

	double Uxx = 3.1;
	double FF = 0.857;
	double SizeSC = 9;
	inline double sq(double x) { return x * x; }
	inline int sq(int x) { return x * x; }


private:
	QList<QGroupBox*> arrBox;

	SolarCellPowerLogic* logic;
	SolarGeometryCalculatorNOAA* sgcnoaaWindow;
	Solar_spectrum_calculator* solSpecCalc;
	PowerCellGraphs* graphsWindow = nullptr; // окно графиков

public slots:
	void showPowerCellGraphs();
private slots:
	bool CheckValidationCellSize();
	bool CheckValidationUoc();
	bool CheckValidationFF();
	void GetAddress(QString& add, QLabel* _label);

	void GetNumPN();
	void input0();
	void input1();
	void input2();
	void input3();
	void input4();
	void input5();

	void setDisabledGroupButton(bool isDisabled);
	void GetAllAMPath(QHash<QString, QString>&Path_with_AM_Files);
	void GetPowerPNrange();
	bool GetSolarCellPower();

};
