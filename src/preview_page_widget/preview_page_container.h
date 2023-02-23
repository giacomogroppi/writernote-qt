#pragma once

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

    void draw(const QVector<int> & pos);
    void newPage();
    void changeDocument();

    void Resize();

    void disableAll();

private:
    void appendToVisible(const QVector<preview_page_item *> &l);
    void appendToVisible(preview_page_item *p);
    void removeAllItemFromLayout();
    int get_index(const preview_page_item *item);
    void setAllDisable();
    void appendNecessary();
    void drawAll();
    void draw(int index);
    QVector<preview_page_item *> _item_not_show;
    QVector<preview_page_item *> _item_show;
    MainWindow *_main;

    preview_page_item *at_not_show(int i);
    preview_page_item *at_show(int i);

    Ui::preview_page_container *ui;

private slots:
    void clickUser(void *_this);

signals:
    void changePage(int index);
};

force_inline void preview_page_container::appendToVisible(const QVector<preview_page_item *> &l)
{
    int i;
    this->_item_show.append(l);

    i = _item_show.length();

    for(i --; i >= 0; i--){
        QObject::connect(at_show(i), &preview_page_item::clickUser,
                         this, &preview_page_container::clickUser);
    }
}

force_inline void preview_page_container::appendToVisible(preview_page_item *p)
{
    this->_item_show.append(p);
    QObject::connect(_item_show.last(), &preview_page_item::clickUser,
                     this, &preview_page_container::clickUser);
}

force_inline preview_page_item * preview_page_container::at_show(int i)
{
    W_ASSERT(i >= 0 and i < _item_show.size());
    return _item_show[i];
}

force_inline preview_page_item * preview_page_container::at_not_show(int i)
{
    return _item_not_show[i];
}
