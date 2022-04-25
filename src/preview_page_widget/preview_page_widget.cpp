#include "preview_page_widget.h"
#include "testing/memtest.h"
#include "ui_preview_page_widget.h"
#include "mainwindow.h"

constexpr int TimerTime = .5 * 1000;

preview_page_widget::preview_page_widget(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_widget)
{
    ui->setupUi(this);

    _container = new preview_page_container(this, mainWindow);
    _timer = new QTimer(this);

    QObject::connect(_timer, &QTimer::timeout, this, &preview_page_widget::endTimer);

    this->_timer->start(TimerTime);

    ui->scrollArea->setWidget(_container);
    ui->scrollArea->setWidgetResizable(true);

    _container->setMinimumHeight(this->height());
    _container->setMinimumWidth(this->width());
    ui->scrollArea->setStyleSheet("QScrollArea { border : 0px solid black;}");
}

preview_page_widget::~preview_page_widget()
{
    delete ui;
}

void preview_page_widget::endTimer()
{
    this->_container->draw(_page_mod);
    _page_mod.clear();

    this->_timer->start(TimerTime);
}

void preview_page_widget::mod(int page)
{
    W_ASSERT(is_order(_page_mod));

    if(likely(is_present_in_list_order(_page_mod, page) >= 0))
        return;

    append_order(_page_mod, page);
}

void preview_page_widget::pageMove()
{
    this->_container->pageMove();
}

void preview_page_widget::newPage()
{
    this->_container->newPage();
}

void preview_page_widget::changeDocument()
{
    this->_container->changeDocument();
}
