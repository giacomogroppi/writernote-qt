#ifndef PREVIEW_PAGE_ITEM_H
#define PREVIEW_PAGE_ITEM_H

#include <QWidget>
#include "qlabel.h"
#include "utils/common_script.h"
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

    void draw(const class page &page, cbool selected);

    static QSize get_size();

    void setInvalid() { _index = -1; };

signals:
   void clickUser(void *_this);

private:
    QLabel *_lab;
    const class page *_page;
    int _index;
    bool _selected;
    Ui::preview_page_item *ui;

    void paint(QPixmap &pix);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
};

#endif // PREVIEW_PAGE_ITEM_H
