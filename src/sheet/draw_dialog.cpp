#include "dialog_sheet.h"

#include "ui_dialog_sheet.h"
#include <QPainter>
#include "../utils/color/setcolor.h"

void dialog_sheet::draw(){
    ui->pushButton_color->setAutoFillBackground(true);
    QPalette pal = ui->pushButton_color->palette();

    pal.setColor(QPalette::Button, setcolor(&style_element.style[current].colore));

    ui->pushButton_color->setPalette(pal);

    ui->pushButton_color->update();


    m_graphicsScene->clear();

    /* draw the line */
    penna.setColor(setcolor(&style_element.style[current].colore));
    penna.setWidthF(style_element.style[current].thickness);

    penna.setStyle(Qt::PenStyle::SolidLine);

    int i;

    short int height = ui->graphicsView->height();
    short int width = ui->graphicsView->width();

    short int  spessore_x, spessore_y;

    spessore_x = spessore_y = 0;

    if(style_element.style[current].nx)
        spessore_x = height / style_element.style[current].nx;

    if(style_element.style[current].ny)
        spessore_y = width / style_element.style[current].ny;

    /* vertical line */
    for(i=1; i<style_element.style[current].nx; i++){
        m_graphicsScene->addLine(spessore_x * i, 0, spessore_x * i, height, penna);
    }

    for(i=1; i<style_element.style[current].ny; i++){
        m_graphicsScene->addLine(0, spessore_y * i, width, spessore_y * i, penna);
    }

}


void dialog_sheet::resizeEvent(QResizeEvent *){
    draw();
}
