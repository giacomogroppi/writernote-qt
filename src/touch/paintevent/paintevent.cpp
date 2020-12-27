#include "../tabletcanvas.h"
#include <QPainter>
#include <QDebug>

void TabletCanvas::paintEvent(QPaintEvent *event){
    if (m_pixmap.isNull())
        initPixmap(0);

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);


    this->zoomon(event);
    this->zoomin(event);

    /* gestisce sia lo spostamento verticale in su e in giù con il mouse e con il touch */
    this->ismoving_f(event, &painter);

    /* se si è arrivati al fondo aggiunge i dati per il nuovo foglio */
    this->disegnofoglio_bool = this->disegnofoglio_bool ||
            data->needtocreatenew() ||
            !data->posizionefoglio.length();

    this->disegnafoglio();

    laod(&painter);

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

void TabletCanvas::laod(QPainter *painter){
    if(!isloading) return;
    qDebug() << "Load";

    /* inizia a disegnare i punti */
    int i_, len;

    len = this->data->x.length();

    m_pixmap.fill(Qt::white);

    for(i_ = 0; i_ < len; i_++)
    {
        if(this->data->y.at(i_) < this->m_pixmap.size().height() && this->data->y.at(i_) >= 0){
            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(i_ && this->data->idtratto.at(i_) == this->data->idtratto.at(i_ - 1) && data->idtratto.at(i_) != -1
                    && data->y.at(i_) != 1
                    && data->y.at(i_) != m_pixmap.height()){
                this->updateBrush_load(data->pressure.at(i_), 127, 127, data->color.at(i_));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(this->data->x.at(i_), this->data->y.at(i_)));
            }
            else if(data->idtratto.at(i_) == -1){
                /* carica i punti della pagina */
                updateBrush_load(data->pressure.at(i_), 0, 0, data->color.at(i_));

                painter->setPen(this->m_pen);
                painter->drawLine(data->x.at(i_), data->y.at(i_)
                                  , data->x.at(i_ + 1), data->y.at(i_ + 1));

                /* deve andare aventi di un punto in quando ogni riga è formata da due punti */
                i_++;
            }

            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }
    this->isloading = false;
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(float pressure, int yTilt, int xTilt, QColor color){
    /*int vValue = int(((yTilt + 60.0) / 120.0) * 255);
    int hValue = int(((xTilt + 60.0) / 120.0) * 255);*/

    /* temporary */
    int vValue = 127;
    int hValue = 127;

    switch (m_lineWidthValuator) {
            case PressureValuator:
                m_pen.setWidthF(pressureToWidth(pressure/2.00));
                break;
            case TiltValuator:
                m_pen.setWidthF(std::max(std::abs(vValue - 127),
                                         std::abs(hValue - 127)) / 12);
                break;
            default:
                m_pen.setWidthF(1);
        }
    m_brush.setColor(color);
    m_pen.setColor(color);

}

