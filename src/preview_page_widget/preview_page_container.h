#ifndef PREVIEW_PAGE_CONTAINER_H
#define PREVIEW_PAGE_CONTAINER_H

#include <QWidget>
#include <QVector>
#include "preview_page_widget/preview_page_item.h"

namespace Ui {
class preview_page_container;
}

class preview_page_container : public QWidget
{
    Q_OBJECT

public:
    explicit preview_page_container(QWidget *parent, class MainWindow *mainWindow);
    ~preview_page_container();

private:
    QVector<preview_page_item *> _item_not_show;
    QVector<preview_page_item *> _item_show;
    Ui::preview_page_container *ui;
};

#endif // PREVIEW_PAGE_CONTAINER_H
