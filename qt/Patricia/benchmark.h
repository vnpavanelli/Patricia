#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QDialog>
#include <vector>
#include <string>
#include <patricia.h>

namespace Ui {
class BenchMark;
}

class BenchMark : public QDialog
{
    Q_OBJECT

public:
    explicit BenchMark(QWidget *parent = 0);
    ~BenchMark();

private slots:
    void on_b_rodar_clicked();

    void on_b_dicionario_clicked();

private:
    Ui::BenchMark *ui;
    std::vector<PayLoad> chaves;
    Patricia arvore;

    void print(const QString&);
    void print(const std::string&s) {
        print (QString(s.c_str()));
    }
    void print(const char* s) {
        print(QString(s));
    }
    double bench_insert(int);
    double bench_insert_map(int);
    double bench_search(int);
    double bench_search_map(int);
    double bench_remove(int);
    double bench_remove_map(int);
};

#endif // BENCHMARK_H
