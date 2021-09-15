#include "element_ui.h"
#include "ui_element_ui.h"
#include "../../preview/preview.h"

#define NONE ""

#define HIDE_CLOUD ui->button_download->setVisible(false);

element_ui::element_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::element_ui)
{
    ui->setupUi(this);
    this->item = new QListWidgetItem;
#ifndef CLOUD
    HIDE_CLOUD
#endif
}

element_ui::~element_ui()
{
    delete ui;
}

void element_ui::setData(last_file *data, int index, QListWidgetItem *item)
{
    m_data = data;
    m_index = index;

    set_main(item);
}

void element_ui::set_main(QListWidgetItem *item)
{
    QString text;
    const int height=175, width = 175;
    QPixmap img(width, height);
    text = m_data->posizione;
    text += "  " + (QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o;

    ui->label_last_edit_time->setText((QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o);

    ui->label_path->setText(m_data->posizione);

    if(m_data->type == TYPE_CLOUD){
        text += "\nCloud";
        ui->label_where->setText("Cloud");
        ui->open_exe->setVisible(false);

#ifdef CLOUD
        this->ui->button_download->setVisible(true);
#endif

    }
    else{
        text += "\nLocal";
        ui->label_where->setText("Local");
#ifdef CLOUD
        this->ui->button_download->setVisible(false);
#endif
    }

    if(m_data->owner.type_user == TYPE_OWNER_YOU){
        text += "             You";
        ui->owner_type->setText("You");

        ui->owner->setText(NONE);
    }
    else{
        text += "";
        ui->owner_type->setText(NONE);
        ui->owner->setText(m_data->owner.name);
    }

    item->setText(text);
    if(!preview::get(img, false, m_data->posizione, width, height)){
        item->setIcon(QIcon(":image/images/not_define.png"));
    }else{
        item->setIcon(img);
    }
}

bool element_ui::event(QEvent *event)
{
    if(event->type() == QEvent::MouseButtonRelease){
        emit on_pressed(this->m_index);
        return true;
    }
    return QWidget::event(event);
}

void element_ui::decrease(){
    this->m_index --;
}

void element_ui::on_button_delete_clicked()
{
    emit deleteIn(this->m_index);
}

void element_ui::on_button_download_clicked()
{
    emit downloadIn(this->m_index);
}

#include "../../utils/mostra_explorer/mostra_explorer.h"

void element_ui::on_open_exe_clicked()
{
    mostra_explorer(m_data->posizione);
}
