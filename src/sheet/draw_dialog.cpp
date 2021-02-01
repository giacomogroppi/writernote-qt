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

    QPainter painter(this);


    painter.drawLine(0, 0, 42, 42);
}
