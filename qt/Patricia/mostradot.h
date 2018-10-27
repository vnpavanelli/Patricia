#ifndef MOSTRADOT_H
#define MOSTRADOT_H

#include <QDialog>

namespace Ui {
class MostraDot;
}

class MostraDot : public QDialog
{
    Q_OBJECT

public:
    explicit MostraDot(QWidget *parent = 0);
    void preencheDot(const QString&);
    ~MostraDot();

private slots:
    void on_fechar_clicked();

    void on_b_copy_clicked();

private:
    Ui::MostraDot *ui;
};

#endif // MOSTRADOT_H
