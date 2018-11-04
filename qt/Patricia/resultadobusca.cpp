#include "resultadobusca.h"
#include "ui_resultadobusca.h"
#include "patricia.h"

void ResultadoBusca::Mostra(const RetornoBusca &r, const QString& chave)
{
    if (r.achou) {
        ui->e_resultado->setText("Encontrada!");
        ui->e_chave->setText(chave);
        ui->e_conteudo->setPlainText(r.payload->conteudo.c_str());
    } else {
        ui->e_resultado->setText("Não encontrada!");
    }
    return;
}

ResultadoBusca::ResultadoBusca(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultadoBusca)
{
    ui->setupUi(this);
}

ResultadoBusca::~ResultadoBusca()
{
    delete ui;
}

void ResultadoBusca::on_b_fechar_clicked()
{
    close();
}
