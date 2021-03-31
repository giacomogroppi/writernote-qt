#ifndef COPY_WIDGETS_H
#define COPY_WIDGETS_H

#include <QWidget>

namespace Ui {
class copy_widgets;
}

class copy_widgets : public QWidget
{
    Q_OBJECT

public:
    explicit copy_widgets(QWidget *parent = nullptr);
    ~copy_widgets();

private:
    Ui::copy_widgets *ui;
};

#endif // COPY_WIDGETS_H
