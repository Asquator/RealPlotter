#include <float.h>
#include <QGraphicsItem>
#include <iostream>
#include <cmath>
#include <array>

#include "plot_scene.h"


PlotScene::PlotScene(QWidget *parent)
    : QGraphicsScene{parent}{

    setSceneRect(-INT_MAX/2, -INT_MAX/2, INT_MAX, INT_MAX);

    addAxes();
}


void PlotScene::addAxes(){
    QPen axesPen(Qt::black);
    axesPen.setWidth(3);

    QGraphicsLineItem *x_axis = new QGraphicsLineItem(-width()/2, 0, width()/2, 0);
    QGraphicsLineItem *y_axis = new QGraphicsLineItem(0, height()/2, 0, -height()/2);

    x_axis->setPen(axesPen);
    y_axis->setPen(axesPen);

    x_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    y_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    addItem(x_axis);
    addItem(y_axis);
}

void PlotScene::drawBackground(QPainter *painter, const QRectF &rect){
    //fill white background
    setBackgroundBrush(Qt::white);
    QGraphicsScene::drawBackground(painter, rect);

    //  std::pair<double, double> penSizes = calcAxesWidth(rect);

    /*
    //draw axes
    QPen pen = QPen(Qt::black);GraphicsLineItem(0, height()/2, 0, -height()/2);

    x_axis->setPen(axesPen);
    y_axis->setPen(axesPen);

    x_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    y_axis->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    addItem(x_axis);
    addItem(y_axis);
}

*/
    drawGrid(painter, rect);
}


using std::fabs; using std::fmod;

void PlotScene::drawGrid(QPainter *painter, const QRectF &rect){

    QPen pen = QPen(Qt::black);
    pen.setCosmetic(true);

    double gap = 1 / gridScale, level, extraRender;
    double top = rect.top(), bottom = rect.bottom(), left = rect.left(), right = rect.right();

    painter->setPen(pen);

    //horizontal

    //addLine(-10,-0.999,10,0.999);
    extraRender = (right - left) * EXTRA_RENDER_FACTOR;

    //iterate from top to bottom and draw lines
    for (level = gap * round(top / gap) - gap; level < bottom + gap; level += gap) {
        painter->drawLine(left - extraRender, level,
                        right + extraRender, level);
    }

    //vertical
    painter->setPen(pen);
    extraRender = (bottom - top) * EXTRA_RENDER_FACTOR;

    //iterate from left to right and draw lines
    for (level = gap * round(left / gap) - gap; level <= right + gap; level += gap) {
        painter->drawLine(level, top - extraRender,
                        level, bottom + extraRender);
    }


}

template <int N> class CircularScaler{
public:
    CircularScaler(std::array<double, N> arr, int startUpscalePos):
        factors(arr), pos{startUpscalePos} {}

    double scaleUp(){
        //take this element as a scale factor
        double ret = factors[pos++];
        if(pos == factors.size())
            pos = 0;

        return ret;
    }

    double scaleDown(){
        //take previous value as a scale factor
        if(--pos < 0)
            pos = factors.size()-1;

        return factors[pos];
    }

private:
    int pos;
    std::array<double, N> factors;
};


void PlotScene::updateScale(double newViewScale){
    static CircularScaler<3> scaler({2,2.5,2}, 0);

    double zoomRatio = newViewScale / gridScale;

    if(zoomRatio >= 2) //zoomed in
        gridScale *= scaler.scaleUp();

    else if(zoomRatio <= 0.5) //zoomed out
        gridScale /= scaler.scaleDown();

}
