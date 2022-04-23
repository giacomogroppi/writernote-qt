#ifndef PREVIEW_PAGE_WIDGET_H
#define PREVIEW_PAGE_WIDGET_H

#include <QWidget>
#include "preview_page_widget/preview_page_container.h"

namespace Ui {
class preview_page_widget;
}

class preview_page_widget : public QWidget
{
    Q_OBJECT

public:
    explicit preview_page_widget(QWidget *parent, class MainWindow *mainwindow);
    ~preview_page_widget();

private:
    preview_page_container * _container;
    Ui::preview_page_widget *ui;
};

#endif // PREVIEW_PAGE_WIDGET_H
