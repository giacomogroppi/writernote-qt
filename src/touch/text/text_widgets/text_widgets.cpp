#include "text_widgets.h"
#include "ui_text_widgets.h"
#include "src/utils/common_error_definition.h"
#include "src/touch/tabletcanvas.h"
#include "src/touch/datastruct/datastruct.h"
#include <QDebug>

text_widgets::text_widgets(QWidget *parent, TabletCanvas *data) :
    QWidget(parent),
    ui(new Ui::text_widgets)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground);

    this->parent = data;
}

text_widgets::~text_widgets()
{
    delete ui;
}

/*
 * return true if there are some point
*/
bool text_widgets::isIn(const QPointF &point)
{
    int i, len;
    len = this->m_list.length();

    double x, y;
    x = point.x();
    y = point.y();

    for(i = 0; i < len; i++)
        if(m_list.at(i).x < x && m_list.at(i).y > y)
            return true;


    return false;
}

int text_widgets::saveData(zip_source_t *file)
{
    int i, len = m_list.length();

    if(zip_source_write(file, &len, sizeof(len)) == -1)
        return ERROR;

    for(i=0; i<len; i++)
        if(zip_source_write(file, &m_list[i], sizeof(pointText)) == -1)
            return ERROR;


    return OK;

}

int text_widgets::loadData(zip_file_t *file)
{
    pointText temp;
    int i, len;

    m_list.clear();

    if(zip_fread(file, &len, sizeof(len)) == -1)
        goto error_;

    for(i=0; i<len; i++){
        if(zip_fread(file, &temp, sizeof(temp)) == -1)
            goto error_;
        m_list.append(temp);
    }

    return OK;

    error_:
    m_list.clear();
    return ERROR;

}

void text_widgets::createNew(const QPointF &point){
    pointText temp;
    double x, y;
    auto hostRect = this->cursor().pos();

    x = point.x();
    y = point.y();


    /*
     * centers the point on where the user clicks
    */
    temp.x = D_X + x;
    temp.y = D_Y + y;

    if(this->isHidden())
        this->show();

    /* he move himself */

    this->move(hostRect);

    m_list.append(temp);

}

void text_widgets::needToDraw(QPainter &painter)
{
    const QPointF &translation = parent->data->datatouch->getPointFirstPage();
    const double zoom = parent->data->datatouch->getZoom();
    QPen pen;

    painter.setRenderHint(QPainter::TextAntialiasing);

    for(const auto &tmp: this->m_list){
        pen.setWidthF(tmp.size);
        pen.setColor(tmp.color.toQColor());

        painter.setPen(pen);

        painter.drawText(tmp.toPoint(), tmp.text);
    }

}



bool text_widgets::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

