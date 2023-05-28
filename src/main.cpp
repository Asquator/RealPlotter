#include "plotwidget.h"
#include "Parser/real_parser.h"

#include <QApplication>

#include <iostream>

#include "line_editor.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    PlotWidget w;
    w.show();

    return a.exec();
}

