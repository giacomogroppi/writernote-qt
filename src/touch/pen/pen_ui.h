#ifndef PEN_UI_H
#define PEN_UI_H

#include <QWidget>

#define SPESSORE 1
#define PRESSIONE 2

#define CONTINUA 2
#define TRATTI 4

#define DEFAULTSPESSORE 1
#define DEFAULT_TYPE SPESSORE
#define DEFAULT_TYPE_TRATTO CONTINUA

namespace Ui {
class pen_ui;
}

class pen_ui : public QWidget
{
    Q_OBJECT

    void save_settings();
    void load_settings();

    int m_type_pen = DEFAULT_TYPE;
    int m_spessore_pen = DEFAULTSPESSORE;
    int m_type_tratto = DEFAULT_TYPE_TRATTO;

public:
    explicit pen_ui(QWidget *parent = nullptr);
    ~pen_ui();

private slots:
    void on_slider_size_valueChanged(int value);

    void on_button_continua_clicked();

    void on_button_tratti_clicked();

    void on_button_pressure_clicked();

    void on_button_size_clicked();

private:
    Ui::pen_ui *ui;

    void list_update();
};

#endif // PEN_UI_H
