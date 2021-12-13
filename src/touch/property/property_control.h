#ifndef PROPERTY_CONTROL_H
#define PROPERTY_CONTROL_H

#include <QWidget>
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

    enum ActionProperty: uchar{
        __copy,
        __cut,
        __paste,
        __delete
    };

    //QRect getPos(const QPoint &topLeft, const QPoint &bottomRight);

#define PROPERTY_SHOW_COPY (1 << 0)
#define PROPERTY_SHOW_CUT (1 << 1)
#define PROPERTY_SHOW_DELETE (1 << 2)
#define PROPERTY_SHOW_PASTE (1 << 3)
    void Show(const QPoint &rect, int whatShow);
    void Hide();

private:
    Ui::property_control *ui;

signals:
    void ActionSelection(property_control::ActionProperty action);
private slots:
    void on_button_copy_clicked();
    void on_button_cut_clicked();
    void on_button_delete_clicked();
    void on_button_paste_clicked();
};

inline void property_control::Hide()
{
    this->hide();
}

#endif // PROPERTY_CONTROL_H
