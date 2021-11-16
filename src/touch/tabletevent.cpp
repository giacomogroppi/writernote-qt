#include "tabletcanvas.h"
#include <QDebug>
#include "../utils/dialog_critic/dialog_critic.h"
#include "square/square.h"
#include "../utils/color/setcolor.h"
#include "../mainwindow.h"
#include "../audioplay/audioplay.h"
#include "../audiorecord/audiorecord.h"

#define MAXPOINT 20

QList<point_s> __tmp;
static void AppendAll(Document &doc, const TabletCanvas *canvas, const bool toTheTop);

bool need_save_auto = false;
bool need_save_tmp = false;

static double DocHeight, DocWidth;

static bool sel;
static bool highlighter_method;
static bool pen_method;
static bool selection_method;
static bool rubber_method;
static bool text_method;

static QEvent::Type eventType;

void TabletCanvas::tabletEvent(QTabletEvent *event){
    const QPointF& pointTouch = event->posF();

    DocHeight = data->datatouch->biggery();
    DocWidth = data->datatouch->biggerx();

    isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;
    sel = true;

    eventType = event->type();

    highlighter_method = event->pointerType() == QTabletEvent::PointerType::Eraser || (medotodiinserimento == e_method::highlighter);
    pen_method = (medotodiinserimento == e_method::pen && !highlighter_method);
    selection_method = (medotodiinserimento == e_method::selection && !highlighter_method);
    rubber_method = (medotodiinserimento == e_method::rubber && !highlighter_method);
    text_method = (medotodiinserimento == e_method::text && !highlighter_method);


    //qDebug() << highlighter_method << pen_method << selection_method << rubber_method << text_method << event->pointerType();

    if(pointTouch.x() > DocWidth || pointTouch.y() > DocHeight){
        /* the user is writing in a part where the sheet is not present. You don't have to save the point. And save the end of the current treatment */
        ManageFinish(event);
        goto end;
    }

    switch (eventType) {
        case QEvent::TabletPress: /* when the user release the tablet */
            ManageStart(event, pointTouch);
            break;
        case QEvent::TabletMove: /* user move the pen */
            if (event->device() == QTabletEvent::RotationStylus)
                updateCursor(event);

#if defined(WIN32) || defined(WIN64)
            this->isdrawing = true;
#endif
            if (m_deviceDown) {
                QPainter painter(&m_pixmap);
                if(pen || highlighter_method){
                    updateBrush(event);

                    //paintPixmap(painter, event);
                }

                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                if(pen_method || highlighter_method){
                    updatelist(event);
                }
                else if(rubber_method){
                    QList<int> *ref = m_rubber->actionRubber(data->datatouch, pointTouch);
                    if(ref->length()){
                        this->triggerNewView(*ref, true);
                    }
                }
                else if(selection_method){
                    if(!m_square->check){ /* it means that the user not select anything */
                        m_square->updatePoint(pointTouch);
                    }
                    else{
                        if(!m_square->isinside(pointTouch)){
                            /* se il tocco non è stato interno */
                            m_square->reset();
                        }
                        else{
                            /* a questo punto può muovere di un delta x e y */
                            m_square->move(pointTouch, data);
                        }
                    }
                    sel = false;
                }else if(text_method){
                    if(m_text_w->isIn(pointTouch)){

                    }
                    else{
                        m_text_w->createNew(pointTouch);
                    }
                }
            }else{
                sel = false;
            }
            break;
        case QEvent::TabletRelease: /* pen leaves the tablet */
            this->ManageFinish(event);
            break;
        default:
            break;
    }

end:

    if(sel){
        m_square->reset();
    }
    if(!sel && selection_method){
        data->datatouch->triggerViewIfVisible(parent->m_audioplayer->getPositionSecond(), parent->m_audioplayer->isPlay());
    }

    update();

    event->accept();
}


inline void TabletCanvas::ManageFinish(QTabletEvent *event){
#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif
    if(m_redoundo)
        m_redoundo->copy();

    if(pen_method || highlighter_method)
        AppendAll(*this->data, this, (highlighter_method) ? m_highlighter->moveToTop() : false);

    if (m_deviceDown && event->buttons() == Qt::NoButton){
        m_deviceDown = false;
        if(selection_method){
            sel = false;

            if(!m_square->check)
                m_square->find(data);
        }

        if(rubber_method && m_rubber->m_type_gomma == rubber_ui::total){
            m_rubber->clearList(data->datatouch);
        }
    }
}

inline void TabletCanvas::ManageStart(QTabletEvent *event, const QPointF &pointTouch){
    if(m_deviceDown)
        return;
    if(pen_method || highlighter_method){
        updatelist(event);
    }
    else if(selection_method){
        if(m_square->check){
            m_square->move(pointTouch, data);
        }
        else{
            m_square->updatePoint(pointTouch);
        }
        sel = false;
    }
    m_deviceDown = true;
    lastPoint.pos = event->pos();
    lastPoint.pressure = event->pressure();
    lastPoint.rotation = event->rotation();
}

static bool need_to_change_color(datastruct *data, int id){
    static uint i, len, how, counterPage;
    static uint lenPage;
    lenPage = data->lengthPage();

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        len = data->at(counterPage)->length();
        for(i=0, how = 0; i<len; i++){
            if(data->at(i, counterPage)->idtratto == id)
                how ++;
        }
    }

    if(!how)
        return false;

    return (how % MAXPOINT) ? 0 : 1;
}

void TabletCanvas::updatelist(QTabletEvent *event){
    static double size;
    static uchar alfa;
    static point_s tmp_point;
    //static QPointF PointFirstPage;

    const QPointF &pointTouch = event->posF();

    //PointFirstPage = this->data->datatouch->getPointFirstPage();

    size = event->pressure();
    alfa = highlighter_method ? m_highlighter->getAlfa() : 255;

    if(!this->m_deviceDown){
        tmp_point.idtratto = data->datatouch->maxId() + 1;
    }
    else{
        if(pen_method && m_pen_ui->m_type_tratto == pen_ui::n_tratto::tratti){
            if(need_to_change_color(data->datatouch, __tmp.last().idtratto)){
                if(m_pen_ui->m_last_color.ok == false){
                    /* save the current color */

                    m_pen_ui->m_last_color.ok = true;
                    m_pen_ui->m_last_color.color = m_color;

                    this->m_color = Qt::white;

                }
                else{
                    /* restore the last color */
                    m_pen_ui->m_last_color.ok = false;
                    this->m_color = m_pen_ui->m_last_color.color;

                }
            }
        }
        tmp_point.idtratto = __tmp.first().idtratto;
        //tmp_point.idtratto = data->datatouch->lastId();
    }

    tmp_point.m_x = pointTouch.x();
    tmp_point.m_y = pointTouch.y();
    tmp_point.m_pressure = highlighter_method ? m_highlighter->getSize(size) : m_pen_ui->getSize(size);
    tmp_point.rotation = event->rotation();
    tmp_point.m_posizioneaudio = time/1000;

    setcolor_struct(&tmp_point.m_color, m_color);

    /*if(alfa != 255){
        qDebug() << "alfa: " << alfa;
    }*/

    tmp_point.m_color.colore[3] = alfa;

    __tmp.append(tmp_point);
}

void AppendAll(Document &doc, const TabletCanvas *canvas, const bool toTheTop){
    uint i;
    const uint lenPoint = __tmp.length();
    point_s *point;
    const auto &PointFirstPage = doc.datatouch->getPointFirstPage();

    for(i = 0; i < lenPoint; i++){
        point = &__tmp.operator[](i);
        point->m_x -= PointFirstPage.x();
        point->m_y -= PointFirstPage.y();
    }

    if(toTheTop)
        doc.datatouch->appendToTheTop(__tmp, canvas->parent->m_audioplayer->getPositionSecond(),
                                      canvas->parent->m_audio_recorder->isRecording());
    else
        doc.datatouch->append(__tmp, canvas->parent->m_audioplayer->getPositionSecond(),
                              canvas->parent->m_audio_recorder->isRecording());

    __tmp.clear();
}
