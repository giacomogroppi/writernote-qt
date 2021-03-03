#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"

#include "load_last_style.h"
#include <QPainter>

#include "../utils/color/color_chooser.h"

#include <QMessageBox>

#include <QInputDialog>

#include "../utils/dialog_critic/dialog_critic.h"
#include "../utils/color/setcolor.h"
#include "string.h"

dialog_sheet::dialog_sheet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_sheet)
{
    ui->setupUi(this);

    style_struct *style_last_save = load_last_style();

    int i;
    style_element.quanti = style_last_save->quanti;

    for(i=0; i<QUANTESTRUCT; i++){
        style_element.style[i] = style_last_save->style[i];
    }


    ui->pushButton_color->setAutoFillBackground(true);
    pal = ui->pushButton_color->palette();

    pal.setColor(
                QPalette::Window, setcolor(&style_element.style[0].colore));

    ui->pushButton_color->setPalette(pal);

    this->current = 0;

    m_graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_graphicsScene);

    this->updateList();

    if(style_element.quanti == 0){
        this->hide(true);
        return;
    }
    setValue();

    draw();
}


/* la funzione è responsabile di nascondere i pulsanti che non si possono cliccare */
void dialog_sheet::hide(bool check){
    /* check == true disable */
    ui->graphicsView->setDisabled(check);
    ui->pushButton->setDisabled(check);
    ui->y->setDisabled(check);
    ui->t->setDisabled(check);
    ui->x->setDisabled(check);
    ui->pushButton_color->setDisabled(check);
}

dialog_sheet::~dialog_sheet()
{
    delete m_graphicsScene;
    delete ui;
}

void dialog_sheet::on_t_valueChanged(int arg1)
{
    style_element.style[current].thickness = arg1;
    draw();
}


void dialog_sheet::on_x_valueChanged(int arg1)
{
    style_element.style[current].nx = arg1;
    draw();
}

void dialog_sheet::on_y_valueChanged(int arg1)
{
    style_element.style[current].ny = arg1;
    draw();
}

inline bool operator==(const style_struct& lhs, const style_struct& rhs)
{
    bool check = lhs.quanti == rhs.quanti;

    if(!check) return false;

    int i;
    for(i=0; i<QUANTESTRUCT; i++){
        if(memcmp(&lhs.style[i].colore, &rhs.style[i].colore, sizeof(colore_s)) != 0)
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

    /* save function */
    this->on_pushButton_3_clicked();

    style_struct *last = load_last_style();

    if(*last == style_element)
        return;


    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);


    int ret = msgBox.exec();

    if(ret == QMessageBox::Cancel){
        delete last;
        return event->ignore();
    }
    else if(ret == QMessageBox::Discard){
        delete last;
        return event->accept();
    }


    save_last_style(&style_element);

    delete last;

    return event->accept();
}

void dialog_sheet::on_pushButton_color_clicked()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    setcolor_struct(&style_element.style[current].colore, color);

    draw();
}

void dialog_sheet::updateList(){
    int i;

    ui->listWidget->clear();

    for(i=0; i<style_element.quanti; i++){
        ui->listWidget->addItem((QString)style_element.style[i].nome);
    }
}

void dialog_sheet::setValue(){
    ui->t->setValue(style_element.style[current].thickness);
    ui->y->setValue(style_element.style[current].ny);
    ui->x->setValue(style_element.style[current].nx);
}

/* need to change item and reset the value */
void dialog_sheet::on_listWidget_itemClicked(QListWidgetItem *)
{
    int position = ui->listWidget->currentIndex().row();
    this->current = position;

    draw();

    setValue();

    /*
     * the buttons for insertion are activated, as
     * previously there were no active styles and
     * they were deactivated
    */
    this->hide(false);
}

/* double click on the item */
void dialog_sheet::on_listWidget_itemDoubleClicked(QListWidgetItem *)
{
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Name"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);
    if(!ok || newName.isEmpty())
        return;

    if(newName.length() > STRNOME)
        return dialog_critic("The maximum number of letters for the word is " + QString::number(STRNOME));

    strcpy(style_element.style[current].nome, newName.toUtf8().constData());

    this->updateList();
}

/* default style */
void dialog_sheet::on_pushButton_clicked()
{
    save_default_drawing(&current);
}

/* save */
void dialog_sheet::on_pushButton_3_clicked()
{
    /*for(i=0, style.quanti = 0; i<QUANTESTRUCT; i++)
        style.quanti += strcmp(style.style[i].nome, DEFAULTNOME) != 0;*/


    save_last_style(&style_element);

}

/* create new style */
void dialog_sheet::on_pushButton_2_clicked()
{
    if(style_element.quanti > QUANTESTRUCT)
        return dialog_critic("You cannot create more than " + QString::number(QUANTESTRUCT));

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Name"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);
    if(!ok || newName.isEmpty()){
        return;
    }


    load_default(&style_element, newName.toUtf8().constData(), style_element.quanti);
    style_element.quanti ++;

    this->updateList();
}
