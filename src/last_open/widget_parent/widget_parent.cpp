#include "widget_parent.h"
#include "ui_widget_parent.h"
#include "../element/element_ui.h"
#include <QDebug>
#include "../last_open.h"

widget_parent::widget_parent(QWidget *parent, last_file *ref, const bool showOnlyName, last_open *parent_sec) :
    QWidget(parent),
    ui(new Ui::widget_parent)
{
    element_ui *el;
    uint i;
    this->m_last_file = ref;

    assert(ref);
    assert(parent_sec != nullptr);

    this->parent = parent_sec;
    ui->setupUi(this);

    for(i=0; i<m_last_file->length(); ++i){
        el = new element_ui(nullptr, &m_last_file->at(i), showOnlyName, i);
        this->m_element.append(el);

        connect(el, &element_ui::on_pressed, parent_sec, &last_open::on_clicked);
        connect(el, &element_ui::deleteIn, parent_sec, &last_open::deleteInElement);

#ifdef CLOUD
        connect(el, &element_ui::downloadIn, parent_sec, &last_open::downloadIn);
#endif

    }

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

void widget_parent::updateList()
{
    uint i, k;
    this->clean();
    if(!m_element.length())
        return;

    const int width_all = this->width();
    const int width_single = this->m_element.first()->width();
    const int space = ui->gridLayout->spacing(); // space for item
    const uint len = width_all / (width_single+space*4);
    const uint len_list = m_last_file->length();

    for(i=0, k=1; (k*i) < len_list; ++i){
        for(k=0; k<len && k < len_list; ++k){
            this->ui->gridLayout->addWidget(m_element.operator[](i+k), i, k);
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
