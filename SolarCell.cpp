#include "SolarCell.h"

SolarCell::~SolarCell()
{
    // если нет специальных действий при удалении, оставь пустым
}


void SolarCell::GetSolarCellPower()
{
    for(int i = 0; i < n_pn; i++ )
    {
        QVector<IntDoubleStruct > vecPN;
        logic->Read2DFile(all_pn.pn[i], vecPN);
        logic->vecSolarCellPN.push_back(vecPN);
    }

}
