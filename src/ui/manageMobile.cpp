#include "mainwindow.h"
#include "manageMobile.h"
#include "core/core.h"
#include "ui_mainwindow.h"
#include "ui/controlluibutton.h"

void mobile::make_ui(MainWindow *parent)
{
    ControllUiButton::Mode mode;

    if(core::is_mobile_view()){
        mode = ControllUiButton::Mode::Tablet;
    }else{
        mode = ControllUiButton::Mode::Computer;
    }

    parent->m_controllUi->setTo(mode);
}
