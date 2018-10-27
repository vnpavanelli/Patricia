#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <patricia.h>
#include <dialog_about.h>
#include <mostradot.h>
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

    void on_actionSobre_triggered();

    void on_actionCarregar_chaves_triggered();

    void on_pushButton_clicked();

    void on_b_dot_clicked();

private:
    Ui::MainWindow *ui;
    Patricia p;
    Dialog_About *dialog_about;
    MostraDot *dialog_dot;
    QString dot;
};

#endif // MAINWINDOW_H
