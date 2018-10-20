#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTemporaryFile>
#include <QTextStream>
#include <QProcess>
#include <QGraphicsPixmapItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b_remover_clicked()
{
    p.Remove(ui->r_chave->text().toStdString());
    on_b_mostrar_clicked();
}

void MainWindow::on_b_inserir_clicked()
{
    p.Insere({ui->i_chave->text().toStdString()  , ui->i_conteudo->text().toStdString()});
    on_b_mostrar_clicked();
}

void MainWindow::on_b_mostrar_clicked()
{
    QTemporaryFile tmpfile;
    if (tmpfile.open()) {
        auto dot = p.GeraDot();
        QTextStream stream(&tmpfile);
        stream << QString(dot.c_str());
    }
    std::cout << "Dot gerado em: " << tmpfile.fileName().toStdString() << std::endl;
    QStringList args;
    args << tmpfile.fileName() << "-Tpng" <<  "-o" << "tmp.png";
    QProcess processo;
    std::cout << "Iniciando processo" << std::endl;
    processo.start("dot", args);
    processo.waitForFinished();
    std::cout << "Processo terminado" << std::endl;
    QGraphicsScene* scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    QImage image("tmp.png");
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    ui->graphicsView->show();
}
