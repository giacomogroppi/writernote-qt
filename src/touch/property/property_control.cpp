#include "property_control.h"
#include "ui_property_control.h"
#include "touch/tabletcanvas.h"
#include <QMouseEvent>
#include <QTabletEvent>

#define SET_PRIVATE_STYLE(button)   button->setStyleSheet("background-color: rgba(255, 255, 255, 255)");

static int is_show = false;

property_control::property_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::property_control)
{
    W_ASSERT(parent->objectName() == "TabletCanvas");
    ui->setupUi(this);

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

    if(likely(flags)){
        this->show();
        this->raise();
    }
    else{
        this->hide();
    }    

    is_show = (bool ) (flags != 0);

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
    QTabletEvent *m;
    QPointF pos, new_point;
    bool result = false;

    //qDebug() << "property_control" << __FUNCTION__ << event->type();

    if(event->type() != QEvent::TabletPress){
        is_show = false;
        result = true;
        return QWidget::event(event);
    }

    DO_IF_DEBUG(Hide());

    m = static_cast<QTabletEvent*>( event );
    pos = m->pos();
    new_point = QWidget::mapTo((QWidget *)parent(), pos.toPoint());

    canvas_send_touch_event(parent(), new_point, QEvent::TabletPress, QTabletEvent::Pen);

    qDebug() << "property_control" << __FUNCTION__ << "Return" << ((result) ? "true" : "false") << this->parent()->objectName();

    if(result)
        event->ignore();

    return result;
}

