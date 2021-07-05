#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::openFirstCopybook(){
    auto item = ui->listWidgetSX->item(0);

    if(!item){
        createFirstCopybook();

        m_currentTitle = m_indice.titolo.first();
        updateTitle(m_currenttitle);
    }else{
        this->on_listWidgetSX_itemDoubleClicked(item);
    }
}

void MainWindow::createFirstCopybook(){
    if(!m_indice.isEmpty())
        return;

    QString name;

    name = getNameCopybook();

    createCopybook(name, true);
    ui->listWidgetSX->setEnabled(true);
}

void MainWindow::createCopybook(const QString &name, const bool touch)
{
    m_indice.titolo.append(name);
    m_currenttitle->m_touch = touch;
    m_currenttitle->nome_copybook = name;

    update_list_copybook();

    contrUi();
}

QString MainWindow::getNameCopybook()
{
    uint len;
    len = m_indice.titolo.length();

    if(!len){
        return NAME_UNSAVE;
    }

    return NAME_UNSAVE + " - " + len;
}

void MainWindow::updateTitle(Document *__curr)
{
    setWindowTitle("Writernote - " + __curr->nome_copybook);
}
