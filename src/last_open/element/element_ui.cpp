#include "element_ui.h"
#include "preview/preview.h"
#include "utils/mostra_explorer/mostra_explorer.h"
#include "ui_element_ui.h"
#include "utils/slash/slash.h"
#include "utils/common_def.h"
#include "utils/get_only_name/get_only_name.h"
#include "utils/mostra_explorer/mostra_explorer.h"

element_ui::element_ui(QWidget *parent, const last_file_s *data, const bool showOnlyName, int m_index, const uchar __showFileOnlyIfExist) :
    QWidget(parent),
    ui(new Ui::element_ui)
{
    ui->setupUi(this);

    this->m_index = m_index;
    this->__showFileOnlyIfExist = __showFileOnlyIfExist;

    assert(data);
    assert(m_index >= 0);

    m_data = data;
    set_main();
    this->showOnlyname(showOnlyName);
}

void element_ui::showOnlyname(const bool showOnlyName){
    if(!showOnlyName)
        return;

    const QString &res = get_only_name::exe(m_data->posizione);
    if(res == "")
        return;

    ui->label_path->setText(res);
}

element_ui::~element_ui()
{
    delete img_widget;
    delete ui;
}

void element_ui::set_main()
{
    const int height = 450, width = 450;
    QPixmap img(width, height);

    ui->label_path->setFont(QFont("Helvetica", 11));
    ui->label_path->setText(m_data->posizione);
    ui->label_last_edit_time->setText((QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o);

    if(m_data->type == TYPE_CLOUD){
        ui->label_where->setText("Cloud");
        ui->button_download->setHidden(false);
    }
    else{
        ui->button_download->setHidden(true);
        ui->label_where->setHidden(true);
    }

#ifdef CLOUD
    if(m_data->owner.type_user == TYPE_OWNER_YOU){
        ui->owner_type->setText("Local");
        ui->owner->setText("You");
    }
    else{
        ui->owner_type->setText("Cloud");
        ui->owner->setText(m_data->owner.name);
    }
#else
    ui->owner_type->setHidden(true);
    ui->owner->setHidden(true);
#endif

    if(!preview::get(img, false, m_data->posizione, width, height)){
        if(this->__showFileOnlyIfExist){
            needToDelete = 1;
            return;
        }
        img = QPixmap(":image/images/not_define.png");
    }

    img_widget = new imageWidget(nullptr, &img);
    this->ui->verticalLayout->addWidget(img_widget, 0);

    needToDelete = 0;
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

void element_ui::on_open_exe_clicked()
{
    mostra_explorer(m_data->posizione);
}
