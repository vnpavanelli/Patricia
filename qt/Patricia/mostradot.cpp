#include "mostradot.h"
#include "ui_mostradot.h"
#include <QClipboard>

MostraDot::MostraDot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MostraDot)
{
    ui->setupUi(this);
}

void MostraDot::preencheDot(const QString &dot)
{
    ui->dotEdit->setPlainText(dot);
}

MostraDot::~MostraDot()
{
    delete ui;
}

void MostraDot::on_fechar_clicked()
{
    close();
}

void MostraDot::on_b_copy_clicked()
{
    QApplication::clipboard()->setText(ui->dotEdit->toPlainText());
}
