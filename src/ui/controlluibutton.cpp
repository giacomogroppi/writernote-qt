#include "controlluibutton.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

ControllUiButton::ControllUiButton(MainWindow *parent)
{
    this->parent = parent;
}

void ControllUiButton::initList()
{
    Ui::MainWindow *ui = parent->ui;
    this->m_button.clear();
    //this->m_button.append()
}
