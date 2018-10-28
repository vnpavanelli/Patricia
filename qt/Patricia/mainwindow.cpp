#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTemporaryFile>
#include <QTextStream>
#include <QProcess>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <dialog_about.h>
#include <QFileDialog>
#include <fstream>
#include <mostradot.h>

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
        dot = p.GeraDot().c_str();
        QTextStream stream(&tmpfile);
        stream << dot;
//        std::cout << "DOT:" << std::endl << dot << std::endl;
    }
//    std::cout << "Dot gerado em: " << tmpfile.fileName().toStdString() << std::endl;
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

void MainWindow::on_actionSobre_triggered()
{
    dialog_about = new Dialog_About();
    dialog_about->show();
}

void MainWindow::on_actionCarregar_chaves_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text file"), "", tr("Text Files (*.txt *.dic)"));
    std::string arquivo = fileName.toStdString();
    std::cout << "Iniciando teste, abrindo arquivo: " << arquivo << std::endl;
    char buffer[256];
    std::ifstream arq(arquivo, std::ifstream::in);
    if (!arq.is_open()) {
        std::cout << "Erro abrindo arquivo!" << std::endl;
        return;
    }
    std::string prefixo("__");
    while (arq.getline(buffer, 255)) {
//        std::cout << "Inserindo: " << buffer << std::endl;
        p.Insere({buffer, prefixo+buffer});
    }
    on_b_mostrar_clicked();
    return;
}

void MainWindow::on_pushButton_clicked()
{
    p.Limpa();
    on_b_mostrar_clicked();
}

void MainWindow::on_b_dot_clicked()
{
    dialog_dot = new MostraDot();
    dialog_dot->preencheDot(dot);
    dialog_dot->show();
}

void MainWindow::on_actionIniciar_triggered()
{
    dialog_benchmark = new BenchMark();
    dialog_benchmark->show();
}
