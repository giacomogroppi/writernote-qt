#include "tabletcanvas.h"
#include "../mainwindow.h"

#include "ui_mainwindow.h"

#include "method/methoddefinition.h"

#include "../utils/color/color_chooser.h"

/* penna */
void MainWindow::on_actionpen_triggered()
{
    this->m_canvas->medotodiinserimento = STILO;
}

/* gomma */
void MainWindow::on_actionrubber_triggered()
{
    if(m_canvas->medotodiinserimento == GOMMA){
        QMenu *menu = new QMenu(this);
        menu->setTitle("Choose input method");

        QAction *cancella_id = new QAction;
        cancella_id->setText("Draw area [beta]");
        menu->addAction(cancella_id);

        QAction *delete_noid = new QAction;
        delete_noid->setText("Just delete what you touch");
        menu->addAction(delete_noid);

        connect(cancella_id, &QAction::triggered, this, [&]{
            this->m_canvas->gomma_cancella = true;
        });
        connect(delete_noid, &QAction::triggered, this, [&]{
            this->m_canvas->gomma_cancella = false;
        });


        auto hostRect = this->cursor().pos();
        menu->move(hostRect.x(), hostRect.y());

        if(!menu->exec()){
            goto delete_;
            return;
        }

        goto delete_;

    delete_:
        delete menu;
        delete cancella_id;
        delete delete_noid;
    }
    this->m_canvas->medotodiinserimento = GOMMA;


}

/* taglia */
void MainWindow::on_actionselezionetext_triggered()
{
    this->m_canvas->medotodiinserimento = SELEZIONE;
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
