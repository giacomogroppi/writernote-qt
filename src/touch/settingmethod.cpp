#include "tabletcanvas.h"
#include "mainwindow.h"
#include "ui/toolbar.h"
#include "ui_mainwindow.h"
#include "utils/color/color_chooser.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "rubber/rubber_ui.h"
#include "utils/color_define_rgb.h"
#include "square/square.h"
#include "core/core.h"

#define SET_CHECK(x) x->setChecked(true)
#define SET_NOT_CHECK(x) x->setChecked(false)

void MainWindow::on_actionhighlighter_triggered()
{
    if(_canvas->_method.isHighlighter()){
        this->m_highlighter->show();

        const auto hostRect = core::get_pos_start_mouse();

        m_highlighter->move(hostRect);
    }

    _canvas->_method.setHighlighter();

    updateTouch();
}

void MainWindow::on_actionpen_triggered()
{
    const auto debug = false;

    WDebug(debug, ((_canvas->_method.isPen()) ? "Selected" : "Not selected"));

    if(_canvas->_method.isPen()){
        this->m_pen->show();
        const auto pos_mouse = core::get_pos_start_mouse();

        m_pen->move(pos_mouse);

        W_ASSERT(m_pen->isVisible());
    }

    _canvas->_method.setPen();

    updateTouch();
}

void MainWindow::on_actionrubber_triggered()
{
    if(_canvas->_method.isRubber()){
        this->m_rubber->show();

        const auto hostRect = core::get_pos_start_mouse();
        m_rubber->move(hostRect);

    }

    _canvas->_method.setRubber();

    updateTouch();
}

void MainWindow::on_actionselezionetext_triggered()
{
    _canvas->_method.setSelection();

    updateTouch();
}

void MainWindow::on_actionLaser_triggered()
{
    _canvas->_method.setLaser();
    _canvas->_color = Qt::red;
    updateTouch();
}

/* insert text */
void MainWindow::on_actioninsertText_triggered()
{
    user_message("This function is not implemented yet, I'm sorry");
    updateTouch();
    return;

    if(_canvas->_method.isText()){
        if(m_text->isHidden()){
            this->m_text->show();
        }
        else{
            this->m_text->hide();
        }

        auto hostRect = this->cursor().pos();
        m_text->move(hostRect);

    }

    _canvas->_method.setText();

    updateTouch();
}

void MainWindow::on_actionchoose_color_triggered()
{
    QColor color = color_chooser();
    if(!color.isValid())
        return;

    _canvas->sceltacolorepenna(color);
}

void TabletCanvas::sceltacolorepenna(const QColor color)
{
    this->_color = color;
    this->_pen.setColor(_color);
}

void MainWindow::updateTouch()
{
    static TabletPenMethod last(true);

    ui->actionpen->setChecked(              _canvas->_method.isPen());
    ui->actionrubber->setChecked(           _canvas->_method.isRubber());
    ui->actionselezionetext->setChecked(    _canvas->_method.isSelection());
    ui->actioninsertText->setChecked(       _canvas->_method.isText());
    ui->actionhighlighter->setChecked(      _canvas->_method.isHighlighter());
    ui->actionLaser->setChecked(            _canvas->_method.isLaser());


    _tool_bar->get_pen_button()->setChecked(            _canvas->_method.isPen());
    _tool_bar->get_rubber_button()->setChecked(         _canvas->_method.isRubber());
    _tool_bar->get_cut_button()->setChecked(            _canvas->_method.isSelection());
    _tool_bar->get_text_button()->setChecked(           _canvas->_method.isText());
    _tool_bar->get_highlighter_button()->setChecked(    _canvas->_method.isHighlighter());
    _tool_bar->get_laser_button()->setChecked(          _canvas->_method.isLaser());

    ui->actionblack->setChecked(_canvas->_color == Qt::black);
    ui->actionwhite->setChecked(_canvas->_color == Qt::white);
    ui->actionyellow->setChecked(_canvas->_color == Qt::yellow);
    ui->actionred->setChecked(_canvas->_color == Qt::red);
    ui->actionbrown_color->setChecked(_canvas->_color == COLOR_BROWN);
    ui->actionpurple->setChecked(_canvas->_color == COLOR_PURPLE);

    if(unlikely(!_canvas->_method.isSelection() and last.isSelection())){
        _canvas->_square->changeInstrument();
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
