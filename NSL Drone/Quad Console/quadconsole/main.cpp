#include <QtGui/QApplication>
#include "quadconsole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    quadconsole w;
    w.show();

    return a.exec();
}
