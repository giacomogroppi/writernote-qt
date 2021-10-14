#include "widget_parent.h"
#include "ui_widget_parent.h"
#include "../element/element_ui.h"
#include <QDebug>
#include "../last_open.h"

widget_parent::widget_parent(QWidget *parent, last_file *ref, const bool showOnlyName,
                             last_open *parent_sec, const uchar __num, const uchar __showFileOnlyIfExist) :
    QWidget(parent),
    ui(new Ui::widget_parent)
{
    element_ui *el;
    uint i;
    uint len = m_last_file->length();

    this->m_last_file = ref;
    this->__num = __num;

    assert(ref);
    assert(parent_sec != nullptr);

    this->parent = parent_sec;
    ui->setupUi(this);

    for(i=0; i < len && i < __num; ++i){
        el = new element_ui(nullptr, &m_last_file->at(i), showOnlyName, i, __showFileOnlyIfExist);
        if(el->needToDelete){
            m_last_file->removeAt(i);
            --i;
            --len;
            delete el;
            continue;
        }
        this->m_element.append(el);

        connect(el, &element_ui::on_pressed, parent_sec, &last_open::on_clicked);
        connect(el, &element_ui::deleteIn, parent_sec, &last_open::deleteInElement);

#ifdef CLOUD
        connect(el, &element_ui::downloadIn, parent_sec, &last_open::downloadIn);
#endif

    }

    if(__showFileOnlyIfExist)
        m_last_file->save_data_setting();

    this->updateList();
}

widget_parent::~widget_parent()
{
    int i;
    for(i=0; i<m_element.length(); ++i){
        delete m_element.at(i);
    }
    delete ui;
}

static int last_width = -1;
void widget_parent::updateList()
{
    uint i, k;
    if(!m_element.length())
        return;

    uint count = 0;
    static const int width_all = this->width();
    static const int width_single = this->m_element.first()->width();
    static const int space = ui->gridLayout->spacing(); // space for item
    static const uint len = width_all / (width_single+space*4);
    static const uint len_list = (m_last_file->length() >= __num) ? __num : m_last_file->length();

    if(last_width == width_all)
        return;

    last_width = width_all;

    this->clean();
    for(i=0; count < len_list && i < len_list; ++i){
        for(k=0; k<len && count < len_list; ++k){
            this->ui->gridLayout->addWidget(m_element.operator[](i*len+k), i, k);
            count ++;
        }
    }
}

void widget_parent::decrease(const int index)
{
    uint i;

    this->clean();
    this->m_last_file->removeAt(index);
    delete this->m_element.at(index);
    this->m_element.removeAt(index);

    for(i=index; i<m_last_file->length(); ++i){
        this->m_element.operator[](i)->decrease();
    }
    this->updateList();
}

void widget_parent::clean()
{
    int i;
    const int count = this->ui->gridLayout->count();

    for(i=0; i < count; ++i){
        ui->gridLayout->removeWidget(this->m_element.at(i));
    }
}

void widget_parent::resizeEvent(QResizeEvent *)
{
    this->updateList();
}
