#include "plotwidget.h"
#include "real_parser.h"

#include <QApplication>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <deque>
#include <iostream>

#include "line_editor.h"

#include <chrono>
#include <thread>

void testLeak(){

  //  RealFunctionLib::RealFunction x= RealFunctionLib::createConstant(1);

    std::cout << "entering" << std::endl;
    std::vector<std::shared_ptr<long double>> vec;

        for(int i = 0; i < 1000000; ++i)
        vec.push_back(std::make_shared<long double>(1000000000.234));

    std::this_thread::sleep_for(std::chrono::seconds(8));
        std::cout << "exiting" <<std::endl;

}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    PlotWidget w;
    w.show();

    return a.exec();
}
