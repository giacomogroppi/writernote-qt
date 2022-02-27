#include "tabletcanvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/color/color_chooser.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "rubber/rubber_ui.h"
#include "utils/color_define_rgb.h"
#include "square/square.h"

#define SET_CHECK(x) x->setChecked(true)
#define SET_NOT_CHECK(x) x->setChecked(false)

void MainWindow::on_actionhighlighter_triggered()
{
    if(_canvas->medotodiinserimento == TabletCanvas::e_method::highlighter){
        this->m_highlighter->show();

        auto hostRect = cursor().pos();
        m_highlighter->move(hostRect);
    }

    _canvas->medotodiinserimento = TabletCanvas::e_method::highlighter;

    updateTouch();
}

void MainWindow::on_actionpen_triggered()
{
    if(_canvas->medotodiinserimento == TabletCanvas::e_method::pen){
        this->m_pen->show();

        auto hostRect = cursor().pos();
        m_pen->move(hostRect);

    }

    _canvas->medotodiinserimento = TabletCanvas::e_method::pen;

    updateTouch();
}

void MainWindow::on_actionrubber_triggered()
{
    if(_canvas->medotodiinserimento == TabletCanvas::e_method::rubber){
        this->m_rubber->show();

        QPoint hostRect = this->cursor().pos();
        m_rubber->move(hostRect);

    }

    this->_canvas->medotodiinserimento = TabletCanvas::e_method::rubber;

    updateTouch();
}

void MainWindow::on_actionselezionetext_triggered()
{
    _canvas->medotodiinserimento = TabletCanvas::e_method::selection;

    updateTouch();
}

void MainWindow::on_actionLaser_triggered()
{
    _canvas->medotodiinserimento = TabletCanvas::e_method::laser;
    _canvas->m_color = Qt::red;
    updateTouch();
}

/* insert text */
void MainWindow::on_actioninsertText_triggered()
{
    user_message("This function is not implemented yet, I'm sorry");
    updateTouch();
    return;

    if(_canvas->medotodiinserimento == TabletCanvas::e_method::text){
        if(m_text->isHidden()){
            this->m_text->show();
        }
        else{
            this->m_text->hide();
        }

        auto hostRect = this->cursor().pos();
        m_text->move(hostRect);

    }

    _canvas->medotodiinserimento = TabletCanvas::e_method::text;

    updateTouch();
}

void MainWindow::on_actionchoose_color_triggered()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    _canvas->sceltacolorepenna(color);
}

void TabletCanvas::sceltacolorepenna(const QColor color){
    this->m_color = color;
    this->m_pen.setColor(m_color);
}

void MainWindow::updateTouch(){
    static TabletCanvas::e_method last = TabletCanvas::pen;

    ui->actionpen->setChecked(              _canvas->medotodiinserimento == TabletCanvas::pen);
    ui->actionrubber->setChecked(           _canvas->medotodiinserimento == TabletCanvas::rubber);
    ui->actionselezionetext->setChecked(    _canvas->medotodiinserimento == TabletCanvas::selection);
    ui->actioninsertText->setChecked(       _canvas->medotodiinserimento == TabletCanvas::text);
    ui->actionhighlighter->setChecked(      _canvas->medotodiinserimento == TabletCanvas::highlighter);
    ui->actionLaser->setChecked(            _canvas->medotodiinserimento == TabletCanvas::laser);

    ui->buttonPen->setChecked(              _canvas->medotodiinserimento == TabletCanvas::pen);
    ui->buttonRubber->setChecked(           _canvas->medotodiinserimento == TabletCanvas::rubber);
    ui->buttonselezionetext->setChecked(    _canvas->medotodiinserimento == TabletCanvas::selection);
    ui->buttonInsertText->setChecked(       _canvas->medotodiinserimento == TabletCanvas::text);
    ui->buttonHighlighter->setChecked(      _canvas->medotodiinserimento == TabletCanvas::highlighter);

    ui->actionblack->setChecked(_canvas->m_color == Qt::black);
    ui->actionwhite->setChecked(_canvas->m_color == Qt::white);
    ui->actionyellow->setChecked(_canvas->m_color == Qt::yellow);
    ui->actionred->setChecked(_canvas->m_color == Qt::red);
    ui->actionbrown_color->setChecked(_canvas->m_color == COLOR_BROWN);
    ui->actionpurple->setChecked(_canvas->m_color == COLOR_PURPLE);

    if(unlikely(_canvas->medotodiinserimento != TabletCanvas::selection
            && last == TabletCanvas::selection)){
        _canvas->m_square->changeInstrument();
    }
}

void MainWindow::on_actionblack_triggered()
{
    this->_canvas->sceltacolorepenna(Qt::black);
    updateTouch();
}

void MainWindow::on_actionwhite_triggered()
{
    this->_canvas->sceltacolorepenna(Qt::white);
    updateTouch();
}

void MainWindow::on_actionyellow_triggered()
{
    this->_canvas->sceltacolorepenna(Qt::yellow);
    updateTouch();
}

void MainWindow::on_actionred_triggered()
{
    this->_canvas->sceltacolorepenna(Qt::red);
    updateTouch();
}

void MainWindow::on_actionbrown_color_triggered()
{
    this->_canvas->sceltacolorepenna(COLOR_BROWN);
    updateTouch();
}

void MainWindow::on_actionpurple_triggered()
{
    this->_canvas->sceltacolorepenna(COLOR_PURPLE);
    updateTouch();
}
