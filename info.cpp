
#include "SolarGeometryCalculatorNOAA.h"

void SolarGeometryCalculatorNOAA::info()
{

	QMessageBox msg;
	msg.setWindowTitle("Information");
	//QTextCodec* codec = QTextCodec::codecForName("KOI8-R");
	QString st = "������������ ������: \n ����� � ����������� �� �� ������ : / latitude / ������.dat \n ����� �� ��������� ���������� ��������� : / spectors / AM.dat \n ����� �� ������������ ��������� : / Data / AM.dat \n �������� �����: \n ����� �������� �� �������: \n 1 p-n �������: �����.dat \n 2 p-n �������: ����������.dat \n ���������� �������� ��� ������: CurrAM on (������).dat \n ���������� �������� ������: Power on (������). dat";
	//st.push_back("������������� ������: \n  ����� � ������ �� �� ������ : /latitude/ ������.dat \n ����� �� ��������� ���������� ��������� : /spectors/ AM.dat");
	//msg.setText(trUtf8("������������� ������: \n  ����� � ������ �� �� ������ : /latitude/ ������.dat \n ����� �� ��������� ���������� ��������� : /spectors/ AM.dat"));
	msg.setText(st);
	msg.exec();
}

void SolarGeometryCalculatorNOAA::Error()
{
	QMessageBox msg;
	msg.setWindowTitle("Error");
	//QTextCodec* codec = QTextCodec::codecForName("KOI8-R");
	QString st = "�� ��� ���� ���������!"; //= path2spec.toLocal8Bit();
	//st.push_back("������������� ������: \n  ����� � ������ �� �� ������ : /latitude/ ������.dat \n ����� �� ��������� ���������� ��������� : /spectors/ AM.dat");
	//msg.setText(trUtf8("������������� ������: \n  ����� � ������ �� �� ������ : /latitude/ ������.dat \n ����� �� ��������� ���������� ��������� : /spectors/ AM.dat"));
	msg.setText(st);
	msg.exec();
}