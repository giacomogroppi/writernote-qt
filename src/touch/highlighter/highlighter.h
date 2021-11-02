#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QDialog>

#include <QList>

namespace Ui {
class highlighter;
}

#define ADD 100.0

struct point_s;
class pen_ui;
class datastruct;

class highlighter : public QDialog
{
    Q_OBJECT

public:
    explicit highlighter(QWidget *parent, bool *same, pen_ui * par);
    ~highlighter();

    uchar getAlfa(){
        return m_data.alfa;
    };
    double getSize(const double pressure);

    bool *same_data = nullptr;
    pen_ui *m_pen = nullptr;

    void moveAll(datastruct *, QList<point_s> &point);

    void updateList();

    bool moveToTop() const;

private slots:
    //void on_slider_actionTriggered(int action);

    void on_button_size_clicked();

    void on_button_pressure_clicked();

    void on_same_data_stateChanged(int arg1);

    void on_checkbox_up_stateChanged(int arg1);

    void on_slider_alfa_valueChanged(int value);

    void on_slider_size_valueChanged(int value);

private:
    struct data{
        uchar alfa;
        double size;
        uchar pressure : 1; /* 1 -> pressure */
        uchar tratto_sotto : 1; /* 1 -> after release the pen move all at first in list */
    };

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::highlighter *ui;

protected:
    bool event(QEvent *) override;
};

inline bool highlighter::moveToTop() const
{
    return this->m_data.tratto_sotto;
}

#endif // HIGHLIGHTER_H
