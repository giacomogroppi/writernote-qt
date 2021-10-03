#include "dialog_sheet.h"

#include "ui_dialog_sheet.h"
#include <QPainter>
#include "../utils/color/setcolor.h"

void dialog_sheet::draw(){
    short int height, width, spessore_x, spessore_y;
    QPalette pal;
    int i;
    const int len = this->style_element.length();

    this->ui->label_init->setHidden(len);
    this->ui->graphicsView->setHidden(!len);
    this->ui->label_color->setHidden(!len);
    this->ui->label_orizzontal->setHidden(!len);
    this->ui->label_thickness->setHidden(!len);
    this->ui->label_vertical->setHidden(!len);
    this->ui->t->setHidden(!len);
    this->ui->x->setHidden(!len);
    this->ui->y->setHidden(!len);
    this->ui->pushButton->setHidden(!len);
    this->ui->pushButton_color->setHidden(!len);

    if(!len)
        return;

    ui->pushButton_color->setAutoFillBackground(true);
    pal = ui->pushButton_color->palette();

    pal.setColor(QPalette::Button, setcolor(style_element.at(current)->colore));

    ui->pushButton_color->setPalette(pal);

    ui->pushButton_color->update();


    m_graphicsScene->clear();

    /* draw the line */
    penna.setColor(setcolor(style_element.at(current)->colore));
    penna.setWidthF(style_element.at(current)->thickness);

    penna.setStyle(Qt::PenStyle::SolidLine);


    height = ui->graphicsView->height();
    width = ui->graphicsView->width();

    spessore_x = spessore_y = 0;

    if(style_element.at(current)->nx)
        spessore_x = height / style_element.at(current)->nx;

    if(style_element.at(current)->ny)
        spessore_y = width / style_element.at(current)->ny;

    /* vertical line */
    for(i=1; i<style_element.at(current)->nx; i++){
        m_graphicsScene->addLine(spessore_x * i, 0, spessore_x * i, height, penna);
    }

    for(i=1; i<style_element.at(current)->ny; i++){
        m_graphicsScene->addLine(0, spessore_y * i, width, spessore_y * i, penna);
    }

}


void dialog_sheet::resizeEvent(QResizeEvent *){
    draw();
}
