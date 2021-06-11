#ifndef UI_HIGHLIGHTER_H
#define UI_HIGHLIGHTER_H

#include <QDialog>

#include <QList>

namespace Ui {
class ui_highlighter;
}

#define ADD 100

class pen_ui;
class datastruct;

class ui_highlighter : public QDialog
{
    Q_OBJECT

public:
    explicit ui_highlighter(QWidget *parent = nullptr, bool *same = nullptr, pen_ui * par = nullptr);
    ~ui_highlighter();

    uchar getAlfa(){
        return m_data.alfa;
    };
    double getSize(const double pressure);

    bool *same_data = nullptr;
    pen_ui *m_pen = nullptr;

    void moveAll(datastruct *);

    void append(int i){
        if(!m_data.tratto_sotto)
            return;
        __data.append(i);
    }

    void updateList();

private slots:
    void on_slider_actionTriggered(int action);

    void on_button_size_clicked();

    void on_button_pressure_clicked();

    void on_slider_size_actionTriggered(int action);

    void on_same_data_stateChanged(int arg1);

    void on_checkbox_up_stateChanged(int arg1);

private:

    QList<int> __data;

    struct data{
        uchar alfa;
        double size;
        uchar pressure : 1; /* 1 -> pressure */
        uchar tratto_sotto : 1; /* 1 -> after release the pen move all at first in list */
    };

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::ui_highlighter *ui;

protected:
    bool event(QEvent *) override;
};

#endif // UI_HIGHLIGHTER_H
