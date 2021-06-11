#ifndef UI_HIGHLIGHTER_H
#define UI_HIGHLIGHTER_H

#include <QDialog>

namespace Ui {
class ui_highlighter;
}

class pen_ui;

class ui_highlighter : public QDialog
{
    Q_OBJECT

public:
    explicit ui_highlighter(QWidget *parent = nullptr, bool *same = nullptr, pen_ui * par = nullptr);
    ~ui_highlighter();

    uchar getAlfa();
    double getSize(const double pressure);

    bool *same_data = nullptr;
    pen_ui *m_pen = nullptr;

    void updateList();

private slots:
    void on_slider_actionTriggered(int action);

    void on_checkBox_stateChanged(int arg1);

    void on_button_size_clicked();

    void on_button_pressure_clicked();

private:
    struct data{
        uchar alfa;
        double size;
        uchar pressure : 1; /* 1 -> pressure */
    };

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::ui_highlighter *ui;

protected:
    bool event(QEvent *) override;
};

#endif // UI_HIGHLIGHTER_H
