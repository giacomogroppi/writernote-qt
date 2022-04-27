#ifndef PREVIEW_PAGE_WIDGET_H
#define PREVIEW_PAGE_WIDGET_H

#include <QWidget>
#include "preview_page_widget/preview_page_container.h"
#include "preview_page_widget/list_options.h"
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

    void pageMove();
    void newPage();
    void changeDocument();
    void mod(int page);

    static list_options *get_list();

private slots:
    void endTimer();
    void changePage(int index);

private:
    QVector<int> _page_mod;

    MainWindow *_main;
    QTimer *_timer;
    preview_page_container * _container;
    Ui::preview_page_widget *ui;
};

#endif // PREVIEW_PAGE_WIDGET_H
