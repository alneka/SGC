#pragma once


#include <QList>
#include <QMap>
#include <QString>


const int Shift = 29;

struct PNEntry {
    QString filePath; // путь к файлу
    bool isRead;       // true, если путь задан
};

class PN {
public:
    PN() {
        // Инициализируем массив пустыми значениями
        for (int i = 0; i < 6; ++i) {
            pn[i].filePath = "";
            pn[i].isRead = false;
        }
    }

    PN(QString& pn0) :PN() { pn[0] = { pn0, true }; }
    PN(QString& pn0, QString& pn1) :PN(pn0) { pn[1] = { pn1, true }; }
    PN(QString& pn0, QString& pn1, QString& pn2) :PN(pn0, pn1) { pn[2] = { pn2, true }; }
    PN(QString& pn0, QString& pn1, QString& pn2, QString& pn3) :PN(pn0, pn1, pn2) { pn[3] = { pn3, true }; }
    PN(QString& pn0, QString& pn1, QString& pn2, QString& pn3, QString& pn4) :PN(pn0, pn1, pn2, pn3) { pn[4] = { pn4, true }; }
    PN(QString& pn0, QString& pn1, QString& pn2, QString& pn3, QString& pn4, QString& pn5) :PN(pn0, pn1, pn2, pn3, pn4) { pn[5] = { pn5, true }; }

    PNEntry pn[6]; // массив из 6 элементов PNEntry
};

class new_PN {
public:
    new_PN() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < Shift; j++)
                this->pn[i][j] = "";
        }
    }
    QString pn[2][Shift];
};

struct IntDoubleStruct
{
    int intData = 0;
    double doubleData = 0;
};

struct SpChPN {
    QVector<double> Ikz;
};
class SpectralCharacteristic {
public:
    SpectralCharacteristic() {
        this->v_AM_name;
        this->v_scpn;
        this->v_IkzMin;
        this->v_IkzBaseAm;
        this->count = 0;
    }
    void push_back(QVector<double>& scpn) {
        this->v_scpn.push_back(scpn);
        this->count++;
    }
    void push_AM_name(QVector<double>& amname) {
        this->v_AM_name = amname;
    }

    void FindIkzMin() {

        for (int i = 0; i < v_AM_name.size(); i++) {
            double min = 100000;
            for (int j = 0; j < count; j++) {
                if (min >= this->v_scpn[j][i]) {
                    min = v_scpn[j][i];

                }
            }
            v_IkzMin.push_back(min);
        }
    }
    QVector<double> v_AM_name;
    QVector<double> v_IkzMin;
    QVector < QVector<double>> v_scpn;
    QVector <double> v_IkzBaseAm;
    int count;
};
