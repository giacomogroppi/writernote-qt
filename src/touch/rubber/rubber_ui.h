#pragma once

#include <QWidget>
#include "currenttitle/document.h"
#include <QPainter>
#include <QPen>
#include <pthread.h>
#include "RubberMethod.h"

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

    void save_settings() const;
    void load_settings();

    RubberMethod::type_rubber _type_gomma = RubberMethod::total;

    void actionRubber(const QPointF &);
    void initRubber(const QPointF &point);
    int endRubber();

    [[nodiscard]] bool is_set() const { return _executer.is_set(); };

private:
    RubberMethod _executer;
    int _size_gomma = DEFAULT_GOMMA_SIZE;
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
    return _executer.reset();
}

