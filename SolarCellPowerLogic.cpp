#include "SolarCellPowerLogic.h"

#include <iostream>
#include <QLineEdit>
#include <QMessageBox>


SolarCellPowerLogic::SolarCellPowerLogic(QObject* parent)
    : QObject(parent)
{
    // сюда можешь добавить инициализацию
}
SolarCellPowerLogic::~SolarCellPowerLogic()
{
}


void SolarCellPowerLogic::validateDataInput()
{

    // Получаем указатель на QLineEdit, который отправил сигнал
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (!edit) {
        qDebug() << "sender() is not a QLineEdit!";
        return;
    }

    // Находим соответствующий QLabel для статуса
    QLabel* statusLabel = statusLabelsMap.value(edit, nullptr);
    if (!statusLabel) {
        qDebug() << "No status label found for editor:" << edit;
        // Можно создать временный QLabel или просто выводить в qDebug
        // Для примера, просто выйдем, если метка не найдена
        statusLabel = new QLabel(); // Временная метка, чтобы не было падения
    }
    //if (!edit->text().isEmpty() && !edit->hasAcceptableInput()) {
    //    // Ввод есть, но он не соответствует валидатору

    //    statusLabel->setStyleSheet("color: red;");
    //}
    //else if (edit->text().isEmpty() && edit->validator() != nullptr) {
    //    // Поле пустое, но если есть валидатор, можно считать это промежуточным состоянием

    //    statusLabel->setStyleSheet("color: orange;");
    //}
    //else if (edit->hasAcceptableInput() || edit->text().isEmpty()) {
    //    // Ввод корректен или поле пустое (и это допустимо)

    //    statusLabel->setStyleSheet("color: green;");
    //    if (edit->text().isEmpty()) { // Если пусто, вернуть исходный текст метки
    //        statusLabel->setStyleSheet("color: orange;");

    //    }
    //}
     // Проверяем валидатор
 // Проверка с валидатором
    const QValidator* validator = edit->validator();
    if (validator) {
        QString text = edit->text();
        int pos = 0;
        QValidator::State state = validator->validate(text, pos);

        switch (state) {
        case QValidator::Acceptable:
            // ✅ Полностью валидный ввод
            statusLabel->setStyleSheet("color: green;");
            break;
        case QValidator::Intermediate:
            // ⚠️ Промежуточный ввод (например, пользователь пишет "0." или ".5")
            statusLabel->setStyleSheet("color: orange;");
            break;
        case QValidator::Invalid:
        default:
            // ❌ Некорректный ввод
            statusLabel->setStyleSheet("color: red;");
            break;
        }
    }
    else {
        // Нет валидатора
        statusLabel->setStyleSheet("color: black;");
    }
}




void SolarCellPowerLogic::info()
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



void SolarCellPowerLogic::Error()
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