#pragma once

#include <QDialog>
#include <QList>
#include "touch/dataTouch/point.h"
#include "HighligterMethod.h"

namespace Ui {
class highlighter;
}

#define ADD 100.0

class pen_ui;
class datastruct;

class highlighter : public QDialog,
                    public HighligterMethod
{
    Q_OBJECT

public:
    explicit highlighter(QWidget *parent, bool &same, pen_ui * par, std::function<int()> getTime,
                         QPen &pen, QColor &color);
    ~highlighter();

    uchar getAlfa() const final;
    double getSize(double pressure);

    bool &_same_data;
    pen_ui *_pen = nullptr;

    void updateList();

private slots:
    void on_button_size_clicked();
    void on_button_pressure_clicked();
    void on_same_data_stateChanged(int arg1);
    void on_slider_alfa_valueChanged(int value);
    void on_slider_size_valueChanged(int value);

private:
    struct data{
        uchar alfa;
        double size;
        uchar pressure : 1; /* 1 -> pressure */
    };

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::highlighter *ui;

protected:
    bool event(QEvent *) override;
};

inline uchar highlighter::getAlfa() const
{
    return m_data.alfa;
}