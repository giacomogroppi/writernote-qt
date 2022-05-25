#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"
#include <QPainter>
#include "utils/color/color_chooser.h"
#include <QMessageBox>
#include <QInputDialog>
#include "utils/dialog_critic/dialog_critic.h"
#include "string.h"
#include "style_struct.h"
#include "fast-sheet/fast_sheet_ui.h"

dialog_sheet::dialog_sheet(QWidget *parent, fast_sheet_ui *fast) :
    QDialog(parent),
    ui(new Ui::dialog_sheet)
{
    this->fast = fast;
    ui->setupUi(this);

    ui->pushButton_color->setAutoFillBackground(true);

    this->current = 0;

    m_graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_graphicsScene);

    this->updateList();

    if(style_element.length() == 0){
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
    style_element.at_mod(current)->thickness = arg1;
    draw();
}


void dialog_sheet::on_x_valueChanged(int arg1)
{
    style_element.at_mod(current)->nx = arg1;
    draw();
}

void dialog_sheet::on_y_valueChanged(int arg1)
{
    style_element.at_mod(current)->ny = arg1;
    draw();
}

void dialog_sheet::closeEvent (QCloseEvent *event){

    /* save function */
    this->on_pushButton_3_clicked();

    style_struct last;

    if(last == style_element)
        return;


    QMessageBox msgBox;
    msgBox.setText(QApplication::tr("The document has been modified."));
    msgBox.setInformativeText(QApplication::tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);


    int ret = msgBox.exec();

    if(ret == QMessageBox::Cancel){
        return event->ignore();
    }
    else if(ret == QMessageBox::Discard){
        return event->accept();
    }


    this->style_element.save();
    this->fast->needToReload();

    return event->accept();
}

void dialog_sheet::on_pushButton_color_clicked()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    style_element.at_mod(current)->colore.fromColor(color);

    draw();
}

void dialog_sheet::updateList(){
    uint i;

    ui->listWidget->clear();

    for(i=0; i<style_element.length(); i++){
        ui->listWidget->addItem((QString)style_element.at(i)->nome);
    }
}

void dialog_sheet::setValue(){
    ui->t->setValue(style_element.at(current)->thickness);
    ui->y->setValue(style_element.at(current)->ny);
    ui->x->setValue(style_element.at(current)->nx);
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
        return dialog_critic(QApplication::tr("The maximum number of letters for the word is %1").arg(QString::number(STRNOME)));

    strncpy(style_element.at_mod(current)->nome, newName.toUtf8().constData(), STRNOME);

    this->updateList();
}

/* default style */
void dialog_sheet::on_pushButton_clicked()
{
    this->style_element.saveDefault(current);
    fast->needToReload();
}

/* save */
void dialog_sheet::on_pushButton_3_clicked()
{
    this->style_element.save();
}

/* create new style */
void dialog_sheet::on_pushButton_2_clicked()
{
    if(style_element.length() >= QUANTESTRUCT)
        return dialog_critic(QApplication::tr("You cannot create more than %1").arg(qstr::number(QUANTESTRUCT)));

    bool ok;
    QString newName = QInputDialog::getText(this, tr("Name"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);
    if(!ok || newName.isEmpty()){
        return;
    }

    this->style_element.createNew(newName);

    this->updateList();
}
