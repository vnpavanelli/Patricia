#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <patricia.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_b_remover_clicked();

    void on_b_inserir_clicked();

    void on_b_mostrar_clicked();

private:
    Ui::MainWindow *ui;
    Patricia p;
};

#endif // MAINWINDOW_H
