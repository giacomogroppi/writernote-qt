#include "preview_page_widget.h"
#include "testing/memtest.h"
#include "ui_preview_page_widget.h"
#include "mainwindow.h"

constexpr int TimerTime = .5 * 1000;

static list_options *_list;

preview_page_widget::preview_page_widget(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::preview_page_widget)
{
    const auto size = preview_page_item::get_size();

    ui->setupUi(this);

    _main = mainWindow;
    _list = new list_options(this);
    _container = new preview_page_container(this, mainWindow);
    _timer = new QTimer(this);

    QObject::connect(_timer, &QTimer::timeout, this, &preview_page_widget::endTimer);
    QObject::connect(_container, &preview_page_container::changePage, this, &preview_page_widget::changePage);

    this->_timer->start(TimerTime);

    ui->scrollArea->setWidget(_container);
    ui->scrollArea->setWidgetResizable(true);

    this->setMinimumWidth(size.width() + 50);
    _container->setMinimumSize(size + QSize(100, 100));

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

void preview_page_widget::changePage(int index)
{
    Q_UNUSED(index);
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
    const auto size = preview_page_item::get_size();
    const auto index = _main->getCurrentDoc()->datatouch->getFirstPageVisible();
    this->ui->scrollArea->ensureVisible(0, size.height() * index, 20, 20);
}

void preview_page_widget::newPage()
{
    this->_container->newPage();
}

void preview_page_widget::changeDocument()
{
    this->_container->changeDocument();
}

list_options * preview_page_widget::get_list()
{
    return _list;
}
