#ifndef PEN_UI_H
#define PEN_UI_H

#include <QWidget>

#define DefaultS 0.30

class ui_highlighter;

struct last_color{
    bool ok;
    QColor color;
};

namespace Ui {
class pen_ui;
}

class pen_ui : public QWidget
{
    Q_OBJECT

public:
    explicit pen_ui(QWidget *parent = nullptr);
    ~pen_ui();

    ui_highlighter * m_highlighter;

    void save_settings();
    void load_settings();

    enum n_tratto: int{
        continua,   /* ------------- */
        tratti      /* - - - - - - - */
    };

    enum n_pressione: int{
        spessore, /* the size of the pen is decide manually */
        pressione /* the size of the pen is decide by the pressure of the pen on the screen */
    };

    double getSize(const double pressure){
        if(m_type_pen == n_pressione::pressione){
            return pressure;
        }
        return m_spessore_pen;
    }

    inline double get_size_private(){
        return m_spessore_pen;
    };

    n_tratto m_type_tratto = n_tratto::continua;

    struct last_color m_last_color;

    /*
     * if true we need to use the same
     *  data for highlighter and pen
    */
    bool same_data;
    void change_data();

private slots:
    void on_slider_size_valueChanged(int value);

    void on_button_continua_clicked();

    void on_button_tratti_clicked();

    void on_button_pressure_clicked();

    void on_button_size_clicked();
    void on_checkBox_stateChanged(int arg1);

private:
    n_pressione m_type_pen = n_pressione::pressione;
    double m_spessore_pen = DefaultS;

    Ui::pen_ui *ui;

    void list_update();

protected:
    bool event(QEvent *) override;
};

#endif // PEN_UI_H
