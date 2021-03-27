#include "tabletcanvas.h"
#include "../mainwindow.h"

#include "ui_mainwindow.h"

#include "../utils/color/color_chooser.h"

#include "rubber/rubber_ui.h"

#define SET_CHECK(x) x->setChecked(true)
#define SET_NOT_CHECK(x) x->setChecked(false)

/* penna */
void MainWindow::on_actionpen_triggered()
{
    if(m_canvas->medotodiinserimento == TabletCanvas::e_method::pen){
        this->m_pen->show();

        auto hostRect = cursor().pos();
        m_pen->move(hostRect);

        SET_CHECK(ui->actionpen);


    }

    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::pen;

    SET_NOT_CHECK(ui->actionrubber);
    SET_NOT_CHECK(ui->actioninsertText);
    SET_NOT_CHECK(ui->actionselezionetext);
}


/* gomma */
void MainWindow::on_actionrubber_triggered()
{
    if(m_canvas->medotodiinserimento == TabletCanvas::e_method::rubber){
        this->m_rubber->show();

        QPoint hostRect = this->cursor().pos();
        m_rubber->move(hostRect);

        SET_CHECK(ui->actionrubber);

    }

    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::rubber;

    SET_NOT_CHECK(ui->actionpen);
    SET_NOT_CHECK(ui->actioninsertText);
    SET_NOT_CHECK(ui->actionselezionetext);
}

/* taglia */
void MainWindow::on_actionselezionetext_triggered()
{
    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::selection;

    SET_CHECK(ui->actionselezionetext);

    SET_NOT_CHECK(ui->actionpen);
    SET_NOT_CHECK(ui->actioninsertText);
    SET_NOT_CHECK(ui->actionrubber);
}

/* insert text */
void MainWindow::on_actioninsertText_triggered()
{
    if(m_canvas->medotodiinserimento == TabletCanvas::e_method::text){
        if(m_text->isHidden()){
            this->m_text->show();
        }
        else{
            this->m_text->hide();
        }

        auto hostRect = this->cursor().pos();
        m_text->move(hostRect);

        SET_CHECK(ui->actioninsertText);


    }
    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::text;

    SET_NOT_CHECK(ui->actionpen);
    SET_NOT_CHECK(ui->actionrubber);
    SET_NOT_CHECK(ui->actionselezionetext);
}

void MainWindow::on_actionchoose_color_triggered()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    m_canvas->sceltacolorepenna(color);
}

void TabletCanvas::sceltacolorepenna(QColor color){
    this->m_color = color;
    this->m_pen.setColor(this->m_color);
}


void MainWindow::on_actionblack_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::black);
}

void MainWindow::on_actionwhite_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::white);
}

void MainWindow::on_actionyellow_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::yellow);
}

void MainWindow::on_actionred_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::red);
}
