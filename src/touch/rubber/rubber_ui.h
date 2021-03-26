#ifndef RUBBER_UI_H
#define RUBBER_UI_H

#include <QWidget>
#include "../../currenttitle/currenttitle_class.h"
#include <QPainter>
#include <QPen>

#define DEFAULT_GOMMA_SIZE 5

#define POSITION_ALFA 3
#define DECREASE 5

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

    int m_size_gomma = DEFAULT_GOMMA_SIZE;
    e_type_rubber m_type_gomma = e_type_rubber::total;

    bool actionRubber(datastruct *, QPointF, QPainter &);
    bool clearList(datastruct *);

private:
    bool isin(double x, double y, double, double , int);

    QPen penna;

    QList<int> gomma_delete_id;

    void update_data();

    Ui::rubber_ui *ui;

protected:
    bool event(QEvent *) override;
private slots:
    void on_totale_button_clicked();
    void on_partial_button_clicked();
};

#endif // RUBBER_UI_H
