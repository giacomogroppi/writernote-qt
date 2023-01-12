#pragma once

#include <QWidget>
#include "touch/tools/Tools.h"

#define DefaultS 0.30

class highlighter;

struct last_color{
    bool ok;
    QColor color;
};

namespace Ui {
class pen_ui;
}

class pen_ui :  public QWidget,
                public Tools
{
    Q_OBJECT

private:
    std::function<int()> _getTime;
    QColor &_color;
    QPen &_pen;

public:
    explicit pen_ui(QWidget *parent, std::function<int()> getTime, QColor &color,
                    QPen &pen);
    ~pen_ui();

    highlighter * m_highlighter = nullptr;

    void save_settings();
    void load_settings();

    enum n_pressione: int{
        spessore, /* the size of the pen is decide manually */
        pressione /* the size of the pen is decide by the pressure of the pen on the screen */
    };

    double getSize(double pressure);

    bool IsPressure();
    void setType(bool pressure);
    void setWidthTratto(double size);

    struct last_color m_last_color;

    /**
     * if true we need to use the same
     *  data for highlighter and pen
    */
    bool same_data;

    void list_update();

    bool touchBegin(const QPointF& point, double size, Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;

private slots:
    void on_slider_size_valueChanged(int value);

    void on_button_pressure_clicked();

    void on_button_size_clicked();
    void on_checkBox_stateChanged(int arg1);

private:
    n_pressione m_type_pen = n_pressione::pressione;
    double m_spessore_pen = DefaultS;

    Ui::pen_ui *ui;

protected:
    bool event(QEvent *) override;
};

inline void pen_ui::setWidthTratto(const double size)
{
    m_spessore_pen = size;
    list_update();
}

inline void pen_ui::setType(bool pressure)
{
    if(pressure){
        m_type_pen = n_pressione::pressione;
    }else{
        m_type_pen = n_pressione::spessore;
    }
}

inline bool pen_ui::IsPressure()
{
    return m_type_pen == n_pressione::pressione;
}

inline double pen_ui::getSize(double pressure)
{
    if(m_type_pen == n_pressione::pressione){
        return pressure;
    }
    return m_spessore_pen;
}