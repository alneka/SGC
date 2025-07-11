#pragma once

#include "PNTransition.h"
#include "SolarGeometryCalculatorNOAA.h"

class SolarCell : public QObject
{
	Q_OBJECT
public:
	
	SolarCell(SolarCellPowerLogic* logic, SolarGeometryCalculatorNOAA* sgcnoaaWindow) : logic(logic), sgcnoaaWindow(sgcnoaaWindow)
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

	};
	~SolarCell();

	double Uxx = 3.1;
	double FF = 0.857;
	double SizeSC = 9;
	inline double sq(double x) { return x * x; }
	inline int sq(int x) { return x * x; }
	int n_pn = 0;

private:
	QList<QGroupBox*> arrBox;
	PN all_pn;
	SolarCellPowerLogic* logic;
	SolarGeometryCalculatorNOAA* sgcnoaaWindow;
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

	void GetSolarCellPower();
};
