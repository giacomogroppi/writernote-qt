#include "preview_page_widget.h"
#include "testing/memtest.h"
#include "ui_preview_page_widget.h"
#include "mainwindow.h"

preview_page_widget::preview_page_widget(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_widget)
{
    _container = new preview_page_container(this, mainWindow);

    ui->setupUi(this);
    ui->scrollArea->setWidget(_container);
    ui->scrollArea->setWidgetResizable(false);
}

preview_page_widget::~preview_page_widget()
{
    delete ui;
}
