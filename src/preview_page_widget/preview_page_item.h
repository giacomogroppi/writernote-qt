#pragma once

#include <QWidget>
#include "qlabel.h"
#include "utils/WCommonScript.h"
#include "utils/platform.h"
#include "core/core.h"
#include "testing/memtest.h"
#include <QPainter>
#include <QPixmap>

namespace Ui {
class preview_page_item;
}

class preview_page_item : public QWidget
{
    Q_OBJECT

public:
    explicit preview_page_item(QWidget *parent);
    ~preview_page_item();

    void draw(const class Page &page, cbool selected);

    static QSize get_size();

    void setInvalid() { _index = -1; };

    friend class preview_page_container;

signals:
   void clickUser(void *_this);

private:
    QPixmap _pix;
    QLabel *_lab;
    const class Page *_page;
    int _index;
    bool _selected;
    Ui::preview_page_item *ui;

    void paint(QPixmap &pix);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
};

