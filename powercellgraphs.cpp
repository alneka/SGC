#include "powercellgraphs.h"
#include "ui_powercellgraphs.h"

PowerCellGraphs::PowerCellGraphs(QWidget* parent) :
    QWidget(parent),
    ui(new Ui_PowerCellGraphs)
{
    ui->setupUi(this);
}

PowerCellGraphs::~PowerCellGraphs()
{
    delete ui;
}
