
#include "SolarCell.h"
/**
 * \Upper block
 * \return
 */
bool SolarCell::CheckValidationCellSize()
{
	sgcnoaaWindow->ui.le_SolarCellSize->setPlaceholderText("0.0 : 300.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 300.0, 2, sgcnoaaWindow->ui.le_SolarCellSize);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	Validator->setLocale(QLocale::C); // запрещаем запятые как разделитель
	sgcnoaaWindow->ui.le_SolarCellSize->setValidator(Validator);
	logic->statusLabelsMap.insert(sgcnoaaWindow->ui.le_SolarCellSize, sgcnoaaWindow->ui.l_CellSize);
	connect(sgcnoaaWindow->ui.le_SolarCellSize, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
	QString text = sgcnoaaWindow->ui.le_SolarCellSize->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}
bool SolarCell::CheckValidationUoc()
{
	sgcnoaaWindow->ui.le_uoc->setPlaceholderText("0.0 : 100.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 100.0, 2, sgcnoaaWindow->ui.le_uoc);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	sgcnoaaWindow->ui.le_uoc->setValidator(Validator);
	Validator->setLocale(QLocale::C); // запрещаем запятые как разделитель
	logic->statusLabelsMap.insert(sgcnoaaWindow->ui.le_uoc, sgcnoaaWindow->ui.l_Uoc);
	connect(sgcnoaaWindow->ui.le_uoc, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
	QString text = sgcnoaaWindow->ui.le_uoc->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}
bool SolarCell::CheckValidationFF()
{
	sgcnoaaWindow->ui.le_FF->setPlaceholderText("0.0 : 1.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 1.0, 3, sgcnoaaWindow->ui.le_FF);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	Validator->setLocale(QLocale::C); // запрещаем запятые как разделитель
	sgcnoaaWindow->ui.le_FF->setValidator(Validator);
	logic->statusLabelsMap.insert(sgcnoaaWindow->ui.le_FF, sgcnoaaWindow->ui.l_FF);
	connect(sgcnoaaWindow->ui.le_FF, &QLineEdit::textChanged, logic, &SolarCellPowerLogic::validateDataInput);
	QString text = sgcnoaaWindow->ui.le_FF->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}


void SolarCell::GetNumPN() {
	if (!CheckValidationFF() || !CheckValidationUoc() || !CheckValidationCellSize()) {
		logic->Error(); return;
	}
	arrBox.clear();
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_3);
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_4);
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_5);
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_6);
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_7);
	arrBox.push_back(sgcnoaaWindow->ui.groupBox_8);
	for (auto& k : arrBox) { k->setDisabled(true); }

	const QString t_Num_pn = sgcnoaaWindow->ui.cB_numPN->currentText();

	sgcnoaaWindow->ui.groupBox_2->setDisabled(false);

	n_pn = t_Num_pn.toInt();

	for (int i = 0; i < n_pn; ++i) {
		arrBox[i]->setDisabled(false);
	}

	bool control = false;
	for (int i = 0; i < n_pn; ++i) {
		if (all_pn.pn[i] == "") control = true;
	}

	if (control == true) {
		setDisabledGroupButton(true);
		
	}
	else {
		setDisabledGroupButton(false);
	
	}

}


/**
 * \Down block
 * \return
 */
void SolarCell::GetAddress(QString& add, QLabel* _label)
{
	//idget* parentWidget = qobject_cast<QWidget*>(this->parent()); // ищем родительский виджет
	add = QFileDialog::getOpenFileName(sgcnoaaWindow, tr("Choose file"), ".//", tr("Dat file (*.dat)"));
	QStringList parts = add.split("/");
	QString lastBit = parts.at(parts.size() - 1);
	_label->setText(lastBit);
}

void SolarCell::input0()
{
	GetAddress(all_pn.pn[0], sgcnoaaWindow->ui.label_9);
	if (n_pn == 1) {
		setDisabledGroupButton(false);
	}
}
void SolarCell::input1()
{
	GetAddress(all_pn.pn[1], sgcnoaaWindow->ui.label_10);
	if (n_pn == 2) {
		setDisabledGroupButton(false);
	}
}
void SolarCell::input2()
{
	GetAddress(all_pn.pn[2], sgcnoaaWindow->ui.label_11);
	if (n_pn == 3) {
		setDisabledGroupButton(false);
	}
}
void SolarCell::input3()
{
	GetAddress(all_pn.pn[3], sgcnoaaWindow->ui.label_19);
	if (n_pn == 4) {
		setDisabledGroupButton(false);
	}
}
void SolarCell::input4()
{
	GetAddress(all_pn.pn[4], sgcnoaaWindow->ui.label_17);
	if (n_pn == 5) {
		setDisabledGroupButton(false);

	}
}
void SolarCell::input5()
{
	GetAddress(all_pn.pn[5], sgcnoaaWindow->ui.label_15);
	if (n_pn >= 6) {
		setDisabledGroupButton(false);
	}
}

void SolarCell::showPowerCellGraphs()
{
	if (!graphsWindow) {
		graphsWindow = new PowerCellGraphs(); // создаем окно
	}
	graphsWindow->show(); // показываем окно
	graphsWindow->raise(); // поднимаем наверх
	graphsWindow->activateWindow(); // делаем активным
}


void SolarCell::setDisabledGroupButton(bool isDisabled)
{
	sgcnoaaWindow->ui.pb_GetSolarCellPower->setDisabled(isDisabled);
	sgcnoaaWindow->ui.pb_GetPowerAMPN->setDisabled(isDisabled);
	sgcnoaaWindow->ui.pb_show_PowerCellGraphs->setDisabled(isDisabled);
}

