#include "dialog_sheet.h"

#include "ui_dialog_sheet.h"
#include <QPainter>

void dialog_sheet::draw(){
    ui->pushButton_color->setAutoFillBackground(true);
    QPalette pal = ui->pushButton_color->palette();
    pal.setColor(QPalette::Button, settaggiocolore(style.style[current].colore));
    ui->pushButton_color->setPalette(pal);

    ui->pushButton_color->update();

    update();
}

void dialog_sheet::paintEvent(QPaintEvent *event){
    penna.setColor(settaggiocolore(style.style[current].colore));
    penna.setWidth(style.style[current].thickness);

    penna.setStyle(Qt::PenStyle::SolidLine);

    //QPainter painter(this->ui->graphicsView);

    QPainter painter(this);

    painter.setPen(QPen(Qt::blue, 40, Qt::PenStyle::SolidLine));

    painter.setPen(penna);

    int i;

    short int height = ui->graphicsView->height();
    short int width = ui->graphicsView->width();

    short int spessore_x = height / style.style[current].nx;
    short int spessore_y = width / style.style[current].ny;


    for(i=1; i<style.style[current].nx; i++){
        painter.drawLine(0, spessore_x * i, height, spessore_x * i);
    }

    for(i=1; i<style.style[current].ny; i++){
        painter.drawLine(spessore_y * i, 0, spessore_y * i, width);
    }

    //painter.end();
    //this->ui->graphicsView->update();
}
