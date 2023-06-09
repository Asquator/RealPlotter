#include <QApplication>
#include "plotwidget.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    PlotWidget w;
    w.show();

    return a.exec();
}

