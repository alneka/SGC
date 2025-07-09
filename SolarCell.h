#pragma once
#include "PNTransition.h"
#include "SolarGeometryCalculatorNOAA.h"

class SolarCell : public  SolarGeometryCalculatorNOAA
{
	Q_OBJECT
public:
	
	SolarCell(QWidget* parent = nullptr) : SolarGeometryCalculatorNOAA(parent)
	{
		//connect(ui.spravka, SIGNAL(triggered()), this, SLOT(info()));
		connect(ui.pb_input_sc_data, SIGNAL(clicked()), this, SLOT(GetNumPN()));
		connect(ui.pb_1_pn, SIGNAL(clicked()), this, SLOT(input0()));
		connect(ui.pb_2_pn, SIGNAL(clicked()), this, SLOT(input1()));
		connect(ui.pb_3_pn, SIGNAL(clicked()), this, SLOT(input2()));
		connect(ui.pb_4_pn, SIGNAL(clicked()), this, SLOT(input3()));
		connect(ui.pb_5_pn, SIGNAL(clicked()), this, SLOT(input4()));
		connect(ui.pb_6_pn, SIGNAL(clicked()), this, SLOT(input5()));

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
};
