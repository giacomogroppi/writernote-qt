#ifndef TEXT_WIDGETS_H
#define TEXT_WIDGETS_H

#include <QWidget>

namespace Ui {
class text_widgets;
}

class text_widgets : public QWidget
{
    Q_OBJECT

public:
    explicit text_widgets(QWidget *parent = nullptr);
    ~text_widgets();

private:
    Ui::text_widgets *ui;
};

#endif // TEXT_WIDGETS_H
