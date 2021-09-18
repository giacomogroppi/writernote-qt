#include "element_ui.h"
#include "../../preview/preview.h"
#include "../../utils/mostra_explorer/mostra_explorer.h"
#include "ui_element_ui.h"

#define NONE ""


element_ui::element_ui(QWidget *parent, const last_file_s *data) :
    QWidget(parent),
    ui(new Ui::element_ui)
{
    ui->setupUi(this);

    assert(data);

    m_data = data;
    set_main();
}

element_ui::~element_ui()
{
    delete this->img_widget;
    delete ui;
}

void element_ui::set_main()
{
    const int height=400, width = 400;
    QPixmap img(width, height);

    ui->label_path->setFont(QFont("Helvetica", 11));
    ui->label_path->setText(m_data->posizione);
    ui->label_last_edit_time->setText((QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o);

    if(m_data->type == TYPE_CLOUD)
        ui->label_where->setText("Cloud");
    else
        ui->label_where->setHidden(true);

#ifdef CLOUD
    if(m_data->owner.type_user == TYPE_OWNER_YOU)
        text += "             You";
    else
        text += "Not you";
#else
    ui->owner_type->setHidden(true);
    ui->owner->setHidden(true);
#endif

    if(!preview::get(img, false, m_data->posizione, width, height)){
        img = QPixmap(":image/images/not_define.png");
    }
    img_widget = new imageWidget(nullptr, &img);
    this->ui->verticalLayout->addWidget(img_widget, 0);
}

bool element_ui::event(QEvent *event)
{
    /*if(event->type() == QEvent::MouseButtonRelease){
        emit on_pressed(this->m_index);
        return true;
    }*/
    return QWidget::event(event);
}

void element_ui::decrease(){
}

void element_ui::on_button_delete_clicked()
{
    //emit deleteIn(this->m_index);
}

void element_ui::on_button_download_clicked()
{
    //emit downloadIn(this->m_index);
}

#include "../../utils/mostra_explorer/mostra_explorer.h"

void element_ui::on_open_exe_clicked()
{
    mostra_explorer(m_data->posizione);
}
