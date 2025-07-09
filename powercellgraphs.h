#pragma once
#ifndef POWERCELLGRAPHS_H
#define POWERCELLGRAPHS_H

#include <QWidget>
#include <ui_powercellgraphs.h>

namespace Ui {
    class PowerCellGraphs;
}

class PowerCellGraphs : public QWidget
{
    Q_OBJECT

public:
    explicit PowerCellGraphs(QWidget* parent = nullptr);
    ~PowerCellGraphs();

private:
    Ui_PowerCellGraphs* ui;
};

#endif // POWERCELLGRAPHS_H