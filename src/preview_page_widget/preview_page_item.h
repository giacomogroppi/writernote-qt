#ifndef PREVIEW_PAGE_ITEM_H
#define PREVIEW_PAGE_ITEM_H

#include <QWidget>
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

    void draw(const class page &page);

private:
    Ui::preview_page_item *ui;
};

#endif // PREVIEW_PAGE_ITEM_H
