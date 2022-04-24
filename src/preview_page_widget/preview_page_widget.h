#ifndef PREVIEW_PAGE_WIDGET_H
#define PREVIEW_PAGE_WIDGET_H

#include <QWidget>
#include "preview_page_widget/preview_page_container.h"
#include <QTimer>

namespace Ui {
class preview_page_widget;
}

class preview_page_widget : public QWidget
{
    Q_OBJECT

public:
    explicit preview_page_widget(QWidget *parent, class MainWindow *mainwindow);
    ~preview_page_widget();

    void Hide();
    void Show();

    void updateList();
    void pageMove();
    void newPage();
    void changeDocument();
    void mod(int page);

private slots:
    void endTimer();

private:
    QVector<int> _page_mod;

    QTimer *_timer;
    preview_page_container * _container;
    Ui::preview_page_widget *ui;
};

force_inline void preview_page_widget::updateList()
{
    this->_container->updatePage();
}

#endif // PREVIEW_PAGE_WIDGET_H
