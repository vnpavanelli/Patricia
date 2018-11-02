#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <patricia.h>
int main(int argc, char *argv[])
{
    int r;
    {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    r=a.exec();
    }
#ifdef DEBUG
    {
        std::cout << "***** Nós vazados *****" << std::endl;
        for (auto j: Patricia::mapa) {
            std::cout << "Nó ID " << j.first << " não liberado: " << j.second->Chave() << std::endl;
        }
    }
#endif
    return r;
}
