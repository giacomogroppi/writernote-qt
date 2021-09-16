#include "element_ui.h"
#include "../../preview/preview.h"
#include "../../utils/mostra_explorer/mostra_explorer.h"
#include "ui_element_ui.h"

#define NONE ""


element_ui::element_ui(QWidget *parent, const last_file *data) :
    QWidget(parent),
    ui(new Ui::element_ui)
{
    ui->setupUi(this);

    this->item = new QListWidgetItem;
    assert(data);

    m_data = data;
    set_main();
}

element_ui::~element_ui()
{
    delete this->item;
}

void element_ui::set_main()
{
    QString text;
    const int height=400, width = 400;
    QPixmap img(width, height);
    text = m_data->posizione;
    text += "  " + (QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o;

    if(m_data->type == TYPE_CLOUD)
        text += "\nCloud";
    else
        text += "\nLocal";

#ifdef CLOUD
    if(m_data->owner.type_user == TYPE_OWNER_YOU)
        text += "             You";
    else
        text += "";
#endif

    item->setText(text);
    if(!preview::get(img, false, m_data->posizione, width, height)){
        item->setIcon(QIcon(":image/images/not_define.png"));
    }else{
        item->setIcon(img);
    }
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
