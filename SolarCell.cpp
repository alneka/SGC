#include "SolarCell.h"
#include <QMap>
#include <QtConcurrent>


// Структура для хранения входных и выходных данных
struct AMProcessingTask {
    QString key;
    QString filePath;
    QVector<IntDoubleStruct> structAM;
    QVector<QMap<QString, double>> resultMap; // один для каждого подэлемента
};
SolarCell::~SolarCell()
{
    // если нет специальных действий при удалении, оставь пустым
}
void SolarCell::GetAllAMPath(QHash<QString, QString>& Path_with_AM_Files)
{  // Создаем QDir для этой папки
    if (logic->Path_to_solar_spec.isEmpty())
    {
        solSpecCalc->CalcSolarSpectrum();
    }
    if (logic->Path_to_solar_spec.isEmpty()) return;
    if (logic->Path_to_solar_spec[0].isEmpty()) return;
	QString folderPath = logic->Path_to_solar_spec[0]; // если вектор с одним элементом
    
	QDir dir(folderPath);

    // Получаем список файлов с расширением .dat
    QStringList fileList = dir.entryList(QStringList() << "*.dat", QDir::Files);

    for (int mapIndex = 0; mapIndex < logic->AMStatData.size(); ++mapIndex)
    {
        const QMap<QString, int>& dataAMSD = logic->AMStatData[mapIndex];

        // Для каждого ключа в AMStatData ищем значение в power_from_file
        for (auto it = dataAMSD.constBegin(); it != dataAMSD.constEnd(); ++it) {
            QString am_value =  it.key();
            QString search_pattern = QString("AM%1").arg(am_value);
            QString matched_file;
            for (const QString& fileName : fileList)
            {
                if (fileName.contains(search_pattern, Qt::CaseInsensitive))
                {
                    Path_with_AM_Files[am_value]= dir.filePath(fileName);
                    //matched_file = dir.filePath(fileName);
                    break;
                }
            }
            //if (!matched_file.isEmpty())
           //     Path_with_AM_Files.push_back(matched_file);
           // else { Path_with_AM_Files.push_back(""); }
        }
    }
}
void SolarCell::GetPowerPNrange()
{
    //if (!logic->GetSolarCellSpectrum()) return;
    //QVector<double> integratePower;
    //QMap<QString, QString> Path_with_AM_Files;
    //GetAllAMPath(Path_with_AM_Files);
    //QHash<QString, QVector<IntDoubleStruct>> fileCache;
    //QVector<QMap<QString, double>> vecPowerPNrange;
    //for (int mapIndex = 0; mapIndex < logic->AMStatData.size(); ++mapIndex)
    //{
    //    const QMap<QString, int>& dataAMSD = logic->AMStatData[mapIndex];
    //    QVector<QMap<QString, double>> resultMap(6);
    //    for (auto it = dataAMSD.constBegin(); it != dataAMSD.constEnd(); ++it) {
    //        const QString& key = it.key();
    //        if (!Path_with_AM_Files.contains(key)) continue;
    //        if (!fileCache.contains(key)) {
    //            QVector<IntDoubleStruct> structAM;
    //            logic->Read2DFile(Path_with_AM_Files[key], structAM);
    //            fileCache[key] = structAM;
    //        }
    //        const QVector<IntDoubleStruct>& structAM = fileCache[key];
    //        for (int i = 0; i < logic->n_pn; i++)
    //        {
    //            double first = logic->vecSolarCellPN[i].first().intData;
    //            double last = logic->vecSolarCellPN[i].last().intData;

    //            // Ищем начало диапазона
    //            auto beginIt = std::lower_bound(structAM.begin(), structAM.end(), first,
    //                [](const IntDoubleStruct& s, double value) {
    //                    return s.intData < value;
    //                });

    //            double power = 0.0;
    //            for (auto itAM = beginIt; itAM != structAM.end() && itAM->intData < last; ++itAM)
    //            {
    //                power += itAM->doubleData;
    //            }
    //            resultMap[i][key] = power;
    //        }

    //    }
    //    for(int pn = 0 ;pn < logic->n_pn; pn++)
    //    {
    //        vecPowerPNrange.append(resultMap[pn]);
    //    }
    //	


    //}
    //    logic->ShowTableWidgetAmStatisticOrPower(sgcnoaaWindow->ui.tw_AMPowerATMCell, vecPowerPNrange, QString("PowerPN"));
    if (!logic->GetSolarCellSpectrum()) return;

    QHash<QString, QString> Path_with_AM_Files;
    GetAllAMPath(Path_with_AM_Files);

    QVector<AMProcessingTask> tasks;

    // Подготовка задач
    for (const QMap<QString, int>& dataAMSD : logic->AMStatData) {
        for (auto it = dataAMSD.constBegin(); it != dataAMSD.constEnd(); ++it) {
            const QString& key = it.key();
            if (!Path_with_AM_Files.contains(key)) continue;

            AMProcessingTask task;
            task.key = key;
            task.filePath = Path_with_AM_Files[key];
            task.resultMap.resize(logic->n_pn);
            tasks.append(task);
        }
    }

    // Функция для параллельной обработки
    auto processTask = [this](AMProcessingTask& task) {
        logic->Read2DFile(task.filePath, task.structAM);

        for (int i = 0; i < logic->n_pn; ++i) {
            double first = logic->vecSolarCellPN[i].first().intData;
            double last = logic->vecSolarCellPN[i].last().intData;

            auto beginIt = std::lower_bound(task.structAM.begin(), task.structAM.end(), first,
                [](const IntDoubleStruct& s, double value) {
                    return s.intData < value;
                });
            auto endIt = std::lower_bound(task.structAM.begin(), task.structAM.end(), last,
                [](const IntDoubleStruct& s, double value) {
                    return s.intData < value;
                });

            double power = 0.0;
            for (auto itAM = beginIt; itAM != endIt; ++itAM) {
                power += itAM->doubleData;
            }
            task.resultMap[i][task.key] = power;
        }
    };

    // Параллельная обработка
    QtConcurrent::blockingMap(tasks, processTask);

    // Собираем результаты по подэлементам
    QVector<QMap<QString, double>> vecPowerPNrange(logic->n_pn);

    for (const AMProcessingTask& task : tasks) {
        for (int i = 0; i < logic->n_pn; ++i) {
            vecPowerPNrange[i].insert(task.resultMap[i]);
        }
    }

    // Отображаем
    logic->ShowTableWidgetAmStatisticOrPower(sgcnoaaWindow->ui.tw_AMPowerATMCell,
        vecPowerPNrange,
        QString("PowerPN"));

}

bool  SolarCell::GetSolarCellPower()
{
    return false;
}
