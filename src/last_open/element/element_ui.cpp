#include "element_ui.h"
#include "../../preview/preview.h"
#include "../../utils/mostra_explorer/mostra_explorer.h"

#define NONE ""


element_ui::element_ui(QWidget *parent) :
    QWidget(parent)
{
    this->item = new QListWidgetItem;

}

element_ui::~element_ui()
{
    delete this->item;
}

void element_ui::setData(last_file *data)
{
    m_data = data;

    set_main();
}

void element_ui::set_main()
{
    QString text;
    const int height=175, width = 175;
    QPixmap img(width, height);
    text = m_data->posizione;
    text += "  " + (QString)m_data->last_modification_g + " " + (QString)m_data->last_modification_o;

    if(m_data->type == TYPE_CLOUD)
        text += "\nCloud";
    else
        text += "\nLocal";


    if(m_data->owner.type_user == TYPE_OWNER_YOU)
        text += "             You";
    else
        text += "";

    item->setText(text);
    if(!preview::get(img, false, m_data->posizione, width, height)){
        item->setIcon(QIcon(":image/images/not_define.png"));
    }else{
        item->setIcon(img);
    }
}
