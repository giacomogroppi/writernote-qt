#include "text_widgets.h"
#include "ui_text_widgets.h"
#include "utils/common_error_definition.h"
#include "touch/tabletcanvas.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "dataread/xmlstruct.h"
#include "datawrite/savefile.h"
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

#define TEXT_WIDGETS_VER 0
int text_widgets::saveData(zip_source_t *file)
{
    int len = m_list.length();
    int flag = 0;
    int ver;

    W_ASSERT(0);

    if(zip_source_write(file, &ver, sizeof(ver)) == -1)
        return ERROR;

    if(zip_source_write(file, &len, sizeof(len)) == -1)
        return ERROR;

    for(const auto &tmp :  m_list){
        flag += zip_source_write(file, &tmp.color, sizeof(tmp.color)) == -1;
        flag += zip_source_write(file, &tmp.size, sizeof(tmp.size)) == -1;
        flag += zip_source_write(file, &tmp.x, sizeof(tmp.x)) == -1;
        flag += zip_source_write(file, &tmp.y, sizeof(tmp.y)) == -1;

        //flag += savefile::save_string(file, tmp.text.constData()) == ERROR;
    }

    return (flag > 0) ? ERROR : OK;
}

int text_widgets::loadData(zip_file_t *file)
{
    pointText tmp;
    int ver, len;
    int flag;

    flag = zip_fread(file, &ver, sizeof(ver)) == -1;
    
    if(ver == 0){
        flag += zip_fread(file, &len, sizeof(len)) == -1;
        for(; len > 0; len --){
            flag += zip_fread(file, &tmp.color, sizeof(tmp.color)) == -1;
            flag += zip_fread(file, &tmp.size, sizeof(tmp.size)) == -1;
            flag += zip_fread(file, &tmp.x, sizeof(tmp.x)) == -1;
            flag += zip_fread(file, &tmp.y, sizeof(tmp.y)) == -1;
            flag += xmlstruct::load_stringa(file, tmp.text) == -1;
        }
    }

    return (flag > 0) ? ERROR : OK;
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
    const QPointF &translation = parent->getDoc()->getPointFirstPage();
    const double zoom = parent->getDoc()->getZoom();
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

