#ifndef GERANDODOT_H
#define GERANDODOT_H

#include <QDialog>

namespace Ui {
class GerandoDot;
}

class GerandoDot : public QDialog
{
    Q_OBJECT

public:
    explicit GerandoDot(QWidget *parent = 0);
    void addText(const QString&);
    ~GerandoDot();

private:
    Ui::GerandoDot *ui;
};

#endif // GERANDODOT_H
