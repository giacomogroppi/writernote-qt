#ifndef PROPERTY_H
#define PROPERTY_H

#include <QWidget>
#include "../copy/copy_selection.h"
#include <QRect>

class Document;

namespace Ui {
class property;
}

class property : public QWidget
{
    Q_OBJECT

public:
    explicit property(QWidget *parent);
    ~property();

    enum Action: uchar{

    };

    QRect getPos(const QPoint &topLeft, const QPoint &bottomRight);

    void Show(const QRect &rect);
    void Hide();

private:
    Ui::property *ui;

signals:
    void Action(property::Action action);
};


#endif // PROPERTY_H
