#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"

#include "load_last_style.h"
#include <QPainter>

#include "../utils/color/color_chooser.h"

#include <QMessageBox>

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

    if(style.quanti == 0){
        load_default(&style);
    }

    ui->pushButton_color->setAutoFillBackground(true); // IMPORTANT!
    pal = ui->pushButton_color->palette();

    pal.setColor(QPalette::Window, settaggiocolore(style.style[0].colore));
    ui->pushButton_color->setPalette(pal);

    /*
    this->pixmaps = new color_pixmap;
    this->pixmapd = new pixmap;

    ui->horizontalLayout_main->insertWidget(0, this->pixmaps);
    ui->horizontalLayout_color->insertWidget(1, this->pixmapd);

    ui->x->setValue(this->style.style[0].nx);
    ui->y->setValue(this->style.style[0].ny);
    ui->t->setValue(this->style.style[0].thickness);

    this->pixmapd->set(this->style.style[0].colore);

    this->pixmaps->set(&this->style.style[0]);
    */
    this->current = 0;

    draw();

    m_graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_graphicsScene);
}

QColor settaggiocolore(int *colore_){
    QColor colore = QColor::fromRgb(colore_[0],
            colore_[1],
            colore_[2],
            colore_[3]);
    return colore;
}

dialog_sheet::~dialog_sheet()
{
    delete pixmapd;
    delete pixmaps;
    delete ui;
}

void dialog_sheet::on_t_valueChanged(int arg1)
{
    this->style.style[current].thickness = arg1;
    //value_change();
    draw();
}


void dialog_sheet::on_x_valueChanged(int arg1)
{
    this->style.style[current].nx = arg1;
    draw();
}

void dialog_sheet::on_y_valueChanged(int arg1)
{
    this->style.style[current].ny = arg1;
    draw();
}

inline bool operator==(const struct style_struct& lhs, const struct style_struct& rhs)
{
    bool check = lhs.quanti == rhs.quanti;

    if(!check) return false;

    int i, k;
    for(i=0; i<QUANTESTRUCT; i++){
        for(k=0; k<4; k++)
            if(lhs.style[i].colore[k] != rhs.style[i].colore[k])
                return false;
        if(lhs.style[i].nx != rhs.style[i].nx)
            return false;

        if(lhs.style[i].ny != rhs.style[i].ny)
            return false;

        if(lhs.style[i].thickness != rhs.style[i].thickness)
            return false;
    }

    return true;
}

void dialog_sheet::closeEvent (QCloseEvent *event){
    style_struct *last = load_last_style();

    if(*last == style){
        delete last;
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);



    int ret = msgBox.exec();

    if(ret != QMessageBox::Save)
    {
        delete last;
        return;
    }

    save_last_style(&style);

    delete last;
    return event->accept();
}

void dialog_sheet::on_pushButton_color_clicked()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    color.getRgb(&style.style[current].colore[0],
            &style.style[current].colore[1],
            &style.style[current].colore[2],
            &style.style[current].colore[3]);

    draw();
}
