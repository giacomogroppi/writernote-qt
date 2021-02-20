#include "redolist.h"

#include "ui_mainwindow.h"

/*
Siccome con un singolo click cambia l'item nella lista, è necessario, in caso non si
voglia passare a un altro copybook, reimpostare quello precedente.
*/
void redolist(MainWindow *parent)
{
    if(parent->m_currentTitle == "")
        return;
    int posizione = parent->m_indice.titolo.indexOf(parent->m_currentTitle, 0);
    parent->ui->listWidgetSX->setCurrentRow(posizione);
}
