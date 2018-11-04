#ifndef RESULTADOBUSCA_H
#define RESULTADOBUSCA_H

#include <QDialog>
#include <patricia.h>

namespace Ui {
class ResultadoBusca;
}

class ResultadoBusca : public QDialog
{
    Q_OBJECT

public:
    void Mostra(const RetornoBusca&, const QString &chave);
    explicit ResultadoBusca(QWidget *parent = 0);
    ~ResultadoBusca();

private slots:
    void on_b_fechar_clicked();

private:
    Ui::ResultadoBusca *ui;
};

#endif // RESULTADOBUSCA_H
