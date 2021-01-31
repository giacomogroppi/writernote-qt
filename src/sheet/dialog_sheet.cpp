#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"

#include "load_last_style.h"

dialog_sheet::dialog_sheet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_sheet)
{
    ui->setupUi(this);

    style_struct *style_last_save = load_last_style();

    int i;
    style.quanti = style_last_save->quanti;

    for(i=0; i<QUANTESTRUCT; i++){
        style.style[i] = style_last_save->style[i];
    }

    /* adjast pixmap */
    this->pixmaps = new color_pixmap;
    this->pixmapd = new pixmap;

    ui->horizontalLayout_main->insertWidget(0, this->pixmaps);
    ui->horizontalLayout_color->insertWidget(1, this->pixmapd);

    ui->x->setValue(this->style.style[0].nx);
    ui->y->setValue(this->style.style[0].ny);
    ui->t->setValue(this->style.style[0].thickness);

    this->pixmapd->set(this->style.style[0].colore);

    this->pixmaps->set(&this->style.style[0]);

    this->current = 0;
}

dialog_sheet::~dialog_sheet()
{
    delete pixmapd;
    delete pixmaps;
    delete ui;
}

void dialog_sheet::value_change(){
    pixmaps->set(&style.style[current]);
    pixmapd->set(style.style[current].colore);
}

void dialog_sheet::on_t_valueChanged(int arg1)
{
    this->style.style[current].thickness = arg1;
    value_change();
}


void dialog_sheet::on_x_valueChanged(int arg1)
{
    this->style.style[current].nx = arg1;
    value_change();
}

void dialog_sheet::on_y_valueChanged(int arg1)
{
    this->style.style[current].nx = arg1;
    value_change();
}

void dialog_sheet::closeEvent (QCloseEvent *event){
    save_last_style(&style);

    return event->accept();
}
