#include "plot_canvas.h"

PlotCanvas::PlotCanvas(QWidget *parent)
    : QWidget{parent}
{
	setMinimumSize(600,400);

	//background color
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
	setPalette(pal);
}

void PlotCanvas::paintEvent(QPaintEvent *event){

}
