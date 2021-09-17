#include "widget_parent.h"
#include "ui_widget_parent.h"
#include "../element/element_ui.h"
#include <QDebug>

widget_parent::widget_parent(QWidget *parent, QList<last_file> *ref) :
    QWidget(parent),
    ui(new Ui::widget_parent)
{
    element_ui *el;
    int i;
    this->m_last_file = ref;

    assert(ref);
    ui->setupUi(this);

    for(i=0; i<m_last_file->length(); ++i){
        el = new element_ui(nullptr, &m_last_file->at(i));
        this->m_element.append(el);
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
    int i, k;
    this->clean();
    if(!m_element.length())
        return;

    const int width_all = this->width();
    const int width_single = this->m_element.first()->width();
    const int space = ui->gridLayout->spacing(); // space for item
    const int len = (width_all / width_single ) * space;

    for(i=0, k=1; (k*i) < m_last_file->length(); ++i){
        for(k=0; k<len && k < m_last_file->length(); ++k){
            this->ui->gridLayout->addWidget(m_element.operator[](i), i, k);
        }
    }
}

void widget_parent::clean()
{
    int i, k;
    const int row = this->ui->gridLayout->rowCount();
    const int column =this->ui->gridLayout->columnCount();
    for(i=0; i<row; ++i){
        for(k=0; k<column; ++k){
            ui->gridLayout->removeWidget(this->m_element.at(i));
        }
    }
}

void widget_parent::resizeEvent(QResizeEvent *)
{
    this->updateList();
}
