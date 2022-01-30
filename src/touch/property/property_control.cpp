#include "property_control.h"
#include "ui_property_control.h"
#include "touch/tabletcanvas.h"
#include <QMouseEvent>

#define SET_PRIVATE_STYLE(button) button->setStyleSheet("background-color: rgba(255, 255, 255, 255)");

property_control::property_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_control)
{
    W_ASSERT(parent->objectName() == "TabletCanvas");
    ui->setupUi(this);

    _canvas = (TabletCanvas *)parent;

    setStyleSheet("background:transparent;");
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    SET_PRIVATE_STYLE(ui->button_copy);
    SET_PRIVATE_STYLE(ui->button_cut);
    SET_PRIVATE_STYLE(ui->button_delete);
    SET_PRIVATE_STYLE(ui->button_paste);
}

property_control::~property_control()
{
    delete ui;
}

void property_control::Show(const QPoint &point, int flags)
{
    ui->button_copy->setEnabled(     (flags & PROPERTY_SHOW_COPY));
    ui->button_cut->setEnabled(      (flags & PROPERTY_SHOW_CUT));
    ui->button_delete->setEnabled(   (flags & PROPERTY_SHOW_DELETE));
    ui->button_paste->setEnabled(    (flags & PROPERTY_SHOW_PASTE));

    if(flags){
        this->show();
        this->raise();
    }
    else{
        this->hide();
    }    

    this->move(point);

}

void property_control::on_button_copy_clicked()
{
    emit ActionSelection(ActionProperty::__copy);
}

void property_control::on_button_cut_clicked()
{
    emit ActionSelection(ActionProperty::__cut);
}

void property_control::on_button_delete_clicked()
{
    emit ActionSelection(ActionProperty::__delete);
}

void property_control::on_button_paste_clicked()
{
    emit ActionSelection(ActionProperty::__paste);
}

bool property_control::event(QEvent *event)
{
    /*
     * For debugging reasons it is always better to hide the
     * window if visible, as if there was a breakpoint while
     * that property is visible it would block all debugging.
    */
    QMouseEvent *m;
    QPointF pos, new_point;
    QMouseEvent *new_event;

#ifdef DEBUGINFO
    bool show;
#endif

    if(likely(event->type() != QEvent::MouseButtonPress))
        goto leave;

#ifdef DEBUGINFO
    show = false;

    if(likely(this->isVisible())){
        show = true;
        this->hide();
    }
#endif

    m = static_cast<QMouseEvent*>( event );
    pos = m->pos();

    new_point = QWidget::mapTo(_canvas, pos.toPoint());

    new_event = new QMouseEvent(QEvent::MouseMove, new_point, Qt::MouseButton::NoButton, Qt::MouseButton::NoButton, Qt::NoModifier);

    qDebug() << "3";

    _canvas->send_mouse_event(new_event);
    //QApplication::sendEvent(_canvas, new_event);

    qDebug() << "4";

    delete new_event;


#ifdef DEBUGINFO
    if(show){
        this->show();
    }
#endif

leave:
    return QWidget::event(event);
}

