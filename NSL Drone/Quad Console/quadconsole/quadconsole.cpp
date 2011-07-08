#include "quadconsole.h"
#include "ui_quadconsole.h"

quadconsole::quadconsole(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::quadconsole)
{
    ui->setupUi(this);
}

quadconsole::~quadconsole()
{
    delete ui;
}
