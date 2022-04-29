#ifndef RUBBER_UI_H
#define RUBBER_UI_H

#include <QWidget>
#include "currenttitle/document.h"
#include <QPainter>
#include <QPen>
#include <pthread.h>

#define DEFAULT_GOMMA_SIZE 5

#define POSITION_ALFA 3
#define DECREASE 2

namespace Ui {
class rubber_ui;
}

class rubber_ui : public QWidget
{
    Q_OBJECT

public:
    explicit rubber_ui(QWidget *parent = nullptr);
    ~rubber_ui();

    void save_settings();
    void load_settings();

    enum e_type_rubber: int{
        total, /* delete all the point with the saim id */
        partial /* delete what the user touch with the pen */
    };

    e_type_rubber _type_gomma = e_type_rubber::total;

    void actionRubber(const QPointF &);
    void initRubber(const QPointF &point);
    int endRubber();

private:
    class TabletCanvas *_canvas;
    int _base;
    PointSettable _last;
    int _size_gomma = DEFAULT_GOMMA_SIZE;
    QList<QVector<int>> _data_to_remove;
    void reset();

    void update_data();

    Ui::rubber_ui *ui;

protected:
    bool event(QEvent *) override;
private slots:
    void on_totale_button_clicked();
    void on_partial_button_clicked();
};

force_inline void rubber_ui::reset()
{
    _base = -1;
    _last.set = false;
    _data_to_remove.clear();
}

#endif // RUBBER_UI_H
