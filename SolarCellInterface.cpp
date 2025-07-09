
#include "SolarCell.h"
/**
 * \Upper block
 * \return
 */
bool SolarCell::CheckValidationCellSize()
{
	ui.lineEdit_6->setPlaceholderText("0.0 : 300.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 300.0, 2, ui.lineEdit_6);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	ui.lineEdit_6->setValidator(Validator);
	statusLabelsMap.insert(ui.lineEdit_6, ui.l_CellSize);
	connect(ui.lineEdit_6, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
	QString text = ui.lineEdit_6->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}
bool SolarCell::CheckValidationUoc()
{
	ui.lineEdit_4->setPlaceholderText("0.0 : 100.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 100.0, 2, ui.lineEdit_4);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	ui.lineEdit_4->setValidator(Validator);
	statusLabelsMap.insert(ui.lineEdit_4, ui.l_Uoc);
	connect(ui.lineEdit_4, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
	QString text = ui.lineEdit_4->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}
bool SolarCell::CheckValidationFF()
{
	ui.lineEdit_5->setPlaceholderText("0.0 : 1.0");
	QDoubleValidator* Validator = new QDoubleValidator(0.0, 1.0, 3, ui.lineEdit_5);
	Validator->setNotation(QDoubleValidator::StandardNotation);
	ui.lineEdit_5->setValidator(Validator);
	statusLabelsMap.insert(ui.lineEdit_5, ui.l_FF);
	connect(ui.lineEdit_5, &QLineEdit::textChanged, this, &SolarGeometryCalculatorNOAA::validateDataSGCInput);
	QString text = ui.lineEdit_5->text();
	int pos = 0;
	return Validator->validate(text, pos) == QValidator::Acceptable;
}


void SolarCell::GetNumPN() {
	if (!CheckValidationFF() || !CheckValidationUoc() || !CheckValidationCellSize()) {
		Error(); return;
	}
	arrBox.clear();
	arrBox.push_back(ui.groupBox_3);
	arrBox.push_back(ui.groupBox_4);
	arrBox.push_back(ui.groupBox_5);
	arrBox.push_back(ui.groupBox_6);
	arrBox.push_back(ui.groupBox_7);
	arrBox.push_back(ui.groupBox_8);
	for (auto& k : arrBox) { k->setDisabled(true); }

	const QString t_Num_pn = ui.comboBox_2->currentText();

	ui.groupBox_2->setDisabled(false);

	n_pn = t_Num_pn.toInt();

	for (int i = 0; i < n_pn; ++i) {
		arrBox[i]->setDisabled(false);
	}

	bool control = false;
	for (int i = 0; i < n_pn; ++i) {
		if (all_pn.pn[i] == "") control = true;
	}

	if (control == true) {
		ui.pb_spect_to_cur->setDisabled(true);
	}
	else {
		ui.pb_spect_to_cur->setDisabled(false);
	}

}


/**
 * \Down block
 * \return
 */
void SolarCell::GetAddress(QString& add, QLabel* _label)
{
	add = QFileDialog::getOpenFileName(this, tr("Choose file"), ".//", tr("Dat file (*.dat)"));
	QStringList parts = add.split("/");
	QString lastBit = parts.at(parts.size() - 1);
	_label->setText(lastBit);
}

void SolarCell::input0()
{
	GetAddress(all_pn.pn[0], ui.label_9);
	if (n_pn == 1) {
		ui.pb_spect_to_cur->setDisabled(false);
	}
}
void SolarCell::input1()
{
	GetAddress(all_pn.pn[1], ui.label_10);
	if (n_pn == 2) {
		ui.pb_spect_to_cur->setDisabled(false);
	}
}
void SolarCell::input2()
{
	GetAddress(all_pn.pn[2], ui.label_11);
	if (n_pn == 3) {
		ui.pb_spect_to_cur->setDisabled(false);
	}
}
void SolarCell::input3()
{
	GetAddress(all_pn.pn[3], ui.label_19);
	if (n_pn == 4) {
		ui.pb_spect_to_cur->setDisabled(false);
	}
}
void SolarCell::input4()
{
	GetAddress(all_pn.pn[4], ui.label_17);
	if (n_pn == 5) {
		ui.pb_spect_to_cur->setDisabled(false);

	}
}
void SolarCell::input5()
{
	GetAddress(all_pn.pn[5], ui.label_15);
	if (n_pn >= 6) {
		ui.pb_spect_to_cur->setDisabled(false);
	}
}

