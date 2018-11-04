#include "gerandodot.h"
#include "ui_gerandodot.h"

GerandoDot::GerandoDot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GerandoDot)
{
    ui->setupUi(this);
}

void GerandoDot::addText(const QString &texto)
{
    ui->plain->appendPlainText(texto);
    return;
}

GerandoDot::~GerandoDot()
{
    delete ui;
}
