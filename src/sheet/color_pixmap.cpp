#include "color_pixmap.h"

#include <QPainter>
#define DELTAX 20
#define DELTAY 20

color_pixmap::color_pixmap(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);
}

void color_pixmap::set(style_struct_S *data){
    this->style = data;

    update();
}


void color_pixmap::paintEvent(QPaintEvent *event){

    this->colore = QColor::fromRgb(
            style->colore[0],
            style->colore[1],
            style->colore[2],
            style->colore[3]);

    pixmap.fill(this->colore);

    this->penna.setColor(colore);
    penna.setWidth(this->style->thickness);

    short int height = this->height();
    short int width = this->width();

    short int spessore_x = height / this->style->nx;
    //short int spessore_y = width / this->style->ny;

    QPainter painter(&this->pixmap);

    int i;
    for(i=1; i<style->nx; i++){
        painter.drawLine(DELTAX, spessore_x * i, height, spessore_x * i);
    }
}
