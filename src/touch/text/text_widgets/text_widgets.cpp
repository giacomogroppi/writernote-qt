#include "text_widgets.h"
#include "ui_text_widgets.h"
#include "../../../utils/common_error_definition.h"
/*
 * this widgets is show when the user want to insert new text
 * or want to modify an element
*/


text_widgets::text_widgets(QWidget *parent, datastruct *data) :
    QWidget(parent),
    ui(new Ui::text_widgets)
{
    ui->setupUi(this);
    this->m_data = data;
}

text_widgets::~text_widgets()
{
    delete ui;
}

/*
 * return true if there are some point
*/
bool text_widgets::isIn(QPointF point)
{
    return false;
}

int text_widgets::saveData(zip_source_t *file)
{
    int i, len = m_lista.length();

    if(zip_source_write(file, &len, sizeof(len)) == -1)
        return ERROR;

    for(i=0; i<len; i++)
        if(zip_source_write(file, &m_lista[i], sizeof(pointText)) == -1)
            return ERROR;


    return OK;

}

int text_widgets::loadData(zip_file_t *file)
{
    m_lista.clear();

    int i, len;

    if(zip_fread(file, &len, sizeof(len)) == -1)
        goto error_;

    pointText temp;

    for(i=0; i<len; i++){
        if(zip_fread(file, &temp, sizeof(temp)) == -1)
            goto error_;
        m_lista.append(temp);
    }

    return OK;

    error_:
    m_lista.clear();
    return ERROR;

}

void text_widgets::createNew(QPointF point){

}
