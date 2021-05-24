#include "tabletcanvas.h"
#include "../mainwindow.h"

#include "ui_mainwindow.h"

#include "../utils/color/color_chooser.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "rubber/rubber_ui.h"

#define SET_CHECK(x) x->setChecked(true)
#define SET_NOT_CHECK(x) x->setChecked(false)

void MainWindow::on_actionpen_triggered()
{
    if(m_canvas->medotodiinserimento == TabletCanvas::e_method::pen){
        this->m_pen->show();

        auto hostRect = cursor().pos();
        m_pen->move(hostRect);

    }

    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::pen;

    updateTouch();
}

void MainWindow::on_actionrubber_triggered()
{
    if(m_canvas->medotodiinserimento == TabletCanvas::e_method::rubber){
        this->m_rubber->show();

        QPoint hostRect = this->cursor().pos();
        m_rubber->move(hostRect);

    }

    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::rubber;

    updateTouch();
}

void MainWindow::on_actionselezionetext_triggered()
{
    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::selection;

    updateTouch();
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

    }
    this->m_canvas->medotodiinserimento = TabletCanvas::e_method::text;

    updateTouch();
}

void MainWindow::on_actionchoose_color_triggered()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    m_canvas->sceltacolorepenna(color);
}

void TabletCanvas::sceltacolorepenna(const QColor color){
    this->m_color = color;
    this->m_pen.setColor(m_color);
}

void MainWindow::updateTouch(){
    ui->actionpen->setChecked(              m_canvas->medotodiinserimento == TabletCanvas::pen);
    ui->actionrubber->setChecked(           m_canvas->medotodiinserimento == TabletCanvas::rubber);
    ui->actionselezionetext->setChecked(    m_canvas->medotodiinserimento == TabletCanvas::selection);
    ui->actioninsertText->setChecked(       m_canvas->medotodiinserimento == TabletCanvas::text);

    ui->actionblack->setChecked(m_canvas->m_color == Qt::black);
    ui->actionwhite->setChecked(m_canvas->m_color == Qt::white);
    ui->actionyellow->setChecked(m_canvas->m_color == Qt::yellow);
    ui->actionred->setChecked(m_canvas->m_color == Qt::red);
}

void MainWindow::on_actionblack_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::black);
    updateTouch();
}

void MainWindow::on_actionwhite_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::white);
    updateTouch();
}

void MainWindow::on_actionyellow_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::yellow);
    updateTouch();
}

void MainWindow::on_actionred_triggered()
{
    this->m_canvas->sceltacolorepenna(Qt::red);
    updateTouch();
}
