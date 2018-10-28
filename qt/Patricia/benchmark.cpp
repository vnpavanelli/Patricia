#include "benchmark.h"
#include "ui_benchmark.h"
#include <QFileDialog>
#include <fstream>
#include <chrono>
#include <ctime>
#include <math.h>
#include <memory.h>

using namespace std;



BenchMark::BenchMark(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BenchMark)
{
    ui->setupUi(this);
}

BenchMark::~BenchMark()
{
    delete ui;
}

void BenchMark::on_b_rodar_clicked()
{
    ui->output->appendPlainText("Rodando...");

    print("Insert:");
    for (int i=10; i<=100; i+=10) {
        double tempo = bench_insert(i);
        double tempo2 = bench_insert_map(i);
        print ("Pct: " + to_string(i) + " Tempo: " + to_string(tempo) + " Tempo(map): " + to_string(tempo2));
    }

    print("Search:");
    for (int i=10; i<=100; i+=10) {
        double tempo = bench_search(i);
        double tempo2 = bench_search_map(i);
        print ("Pct: " + to_string(i) + " Tempo: " + to_string(tempo) + " Tempo(map): " + to_string(tempo2));
    }

    print("Delete:");
    for (int i=10; i<=100; i+=10) {
        double tempo = bench_remove(i);
        double tempo2 = bench_remove_map(i);
        print ("Pct: " + to_string(i) + " Tempo: " + to_string(tempo) + " Tempo(map): " + to_string(tempo2));
    }

    print ("Benchmark terminado");
}

void BenchMark::on_b_dicionario_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text file"), "", tr("Text Files (*.txt *.dic)"));
    ui->output->appendPlainText("Abrindo arquivo: " + fileName);
    std::ifstream arq(fileName.toStdString(), std::ifstream::in);
    if (!arq.is_open()) {
        print("Erro abrindo arquivo!");
        return;
    }
    char buffer[256];
    chaves.clear();
    while (arq.getline(buffer, 255)) {
        chaves.push_back(buffer);
    }
    print ("Inseridas " + std::to_string(chaves.size()) + " chaves");
    return;
}

void BenchMark::print (const QString& s) {
    ui->output->appendPlainText(s);
    QApplication::processEvents();
}

double BenchMark::bench_insert(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    arvore.Limpa();
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        arvore.Insere(j);
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    arvore.Limpa();
    return elapsed_seconds.count();
}

double BenchMark::bench_insert_map(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    std::map<std::string, std::string> mapa;
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        mapa[j] = j;
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    return elapsed_seconds.count();
}

double BenchMark::bench_search(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    arvore.Limpa();
    for (auto const& j: chaves) arvore.Insere(j);
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        auto tmp = arvore.Busca(j);
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    arvore.Limpa();
    return elapsed_seconds.count();

}

double BenchMark::bench_search_map(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    std::map<std::string, std::string> mapa;
    for (auto const& j: chaves) {
        mapa[j] = j;
    }
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        auto tmp = mapa.at(j);
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    return elapsed_seconds.count();
}

double BenchMark::bench_remove(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    arvore.Limpa();
    for (auto const& j: chaves) arvore.Insere(j);
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        arvore.Remove(j);
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    arvore.Limpa();
    return elapsed_seconds.count();

}

double BenchMark::bench_remove_map(int pct)
{
    int i = round(chaves.size()*((double) pct/100.0));
    std::vector<std::string> tmp;
    std::map<std::string, std::string> mapa;
    for (auto const& j: chaves) {
        mapa[j] = j;
    }
    auto start = std::chrono::system_clock::now();
    for (auto const& j: chaves) {
        mapa.erase(j);
        if (--i <= 0) break;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    return elapsed_seconds.count();
}
