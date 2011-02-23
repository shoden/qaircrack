#include <QtGui/QApplication>
#include "qaircrack.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QAircrack w;
    w.show();
    return a.exec();
}
