#include "plot_scene.h"

PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}
{
    setBackgroundBrush(Qt::white);

    /*
	//background color
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);*/
}
