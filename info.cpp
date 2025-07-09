
#include "SolarGeometryCalculatorNOAA.h"

void SolarGeometryCalculatorNOAA::info()
{

	QMessageBox msg;
	msg.setWindowTitle("Information");
	//QTextCodec* codec = QTextCodec::codecForName("KOI8-R");
	QString st = "Расположение данных: \n Файлы с количеством АМ на широте : / latitude / широта.dat \n Файлы со спектрами солнечного излучения : / spectors / AM.dat \n Файлы со стандартными спектрами : / Data / AM.dat \n Выходные файлы: \n Файлы спектров со сдвигом: \n 1 p-n переход: Сдвиг.dat \n 2 p-n переход: СдвигСдвиг.dat \n Результаты расчетов без сдвига: CurrAM on (ШИРОТА).dat \n Результаты расчетов сдвига: Power on (ШИРОТА). dat";
	//st.push_back("Рассположение данных: \n  Файлы с числом АМ на широте : /latitude/ широта.dat \n Файлы со спектрами солнечного излучения : /spectors/ AM.dat");
	//msg.setText(trUtf8("Рассположение данных: \n  Файлы с числом АМ на широте : /latitude/ широта.dat \n Файлы со спектрами солнечного излучения : /spectors/ AM.dat"));
	msg.setText(st);
	msg.exec();
}

void SolarGeometryCalculatorNOAA::Error()
{
	QMessageBox msg;
	msg.setWindowTitle("Error");
	//QTextCodec* codec = QTextCodec::codecForName("KOI8-R");
	QString st = "Не все поля заполнены!"; //= path2spec.toLocal8Bit();
	//st.push_back("Рассположение данных: \n  Файлы с числом АМ на широте : /latitude/ широта.dat \n Файлы со спектрами солнечного излучения : /spectors/ AM.dat");
	//msg.setText(trUtf8("Рассположение данных: \n  Файлы с числом АМ на широте : /latitude/ широта.dat \n Файлы со спектрами солнечного излучения : /spectors/ AM.dat"));
	msg.setText(st);
	msg.exec();
}