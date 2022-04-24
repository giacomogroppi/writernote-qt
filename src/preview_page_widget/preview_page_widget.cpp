#include "preview_page_widget.h"
#include "testing/memtest.h"
#include "ui_preview_page_widget.h"
#include "mainwindow.h"

constexpr int TimerTime = 0.5 * 1000;

preview_page_widget::preview_page_widget(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_widget)
{
    _container = new preview_page_container(this, mainWindow);
    _timer = new QTimer(this);

    QObject::connect(_timer, &QTimer::timeout, this, &preview_page_widget::endTimer);

    this->_timer->start(TimerTime);

    ui->setupUi(this);
    ui->scrollArea->setWidget(_container);
    ui->scrollArea->setWidgetResizable(false);
}

preview_page_widget::~preview_page_widget()
{
    delete ui;
}

void preview_page_widget::endTimer()
{
    this->_container->draw(_page_mod);

    this->_timer->start(TimerTime);
}

void preview_page_widget::mod(int page)
{
    W_ASSERT(is_order(_page_mod));

    if(likely(is_present_in_list_order(_page_mod, page)))
        return;

    append_order(_page_mod, page);
}

void preview_page_widget::pageChange()
{

}
