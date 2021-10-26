#ifndef PROPERTY_CONTROL_H
#define PROPERTY_CONTROL_H

#include <QWidget>
#include "../copy/copy_selection.h"
#include <QRect>

class Document;

namespace Ui {
class property_control;
}

class property_control : public QWidget
{
    Q_OBJECT

public:
    explicit property_control(QWidget *parent);
    ~property_control();

    enum Action: uchar{

    };

    QRect getPos(const QPoint &topLeft, const QPoint &bottomRight);

    void Show(const QRect &rect);
    void Hide();

private:
    Ui::property_control *ui;

signals:
    void ActionSelection(property_control::Action action);
};


#endif // PROPERTY_CONTROL_H
