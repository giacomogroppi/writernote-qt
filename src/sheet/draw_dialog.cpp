#include "dialog_sheet.h"

#include "ui_dialog_sheet.h"
#include <QPainter>

#include <QDebug>

void dialog_sheet::draw(){
    ui->pushButton_color->setAutoFillBackground(true);
    QPalette pal = ui->pushButton_color->palette();
    pal.setColor(QPalette::Button, settaggiocolore(style.style[current].colore));
    ui->pushButton_color->setPalette(pal);

    ui->pushButton_color->update();


    m_graphicsScene->clear();

    /* draw the line */
    penna.setColor(settaggiocolore(style.style[current].colore));
    penna.setWidth(style.style[current].thickness);

    penna.setStyle(Qt::PenStyle::SolidLine);

    int i;

    short int height = ui->graphicsView->height();
    short int width = ui->graphicsView->width();

    short int spessore_x = height / style.style[current].nx;
    short int spessore_y = width / style.style[current].ny;

    //m_graphicsScene->addLine(0, 0, 200, 200, penna);

    /* vertical line */
    for(i=1; i<style.style[current].nx; i++){
        m_graphicsScene->addLine(spessore_x * i, 0, spessore_x * i, height, penna);
    }

    for(i=1; i<style.style[current].ny; i++){
        m_graphicsScene->addLine(0, spessore_y * i, width, spessore_y * i, penna);
    }

}


void dialog_sheet::resizeEvent(QResizeEvent *){
    draw();
}
