#ifndef FAST_SHEET_UI_H
#define FAST_SHEET_UI_H

#include <QWidget>
#include "../load_last_style.h"
#include <QEvent>

namespace Ui {
class fast_sheet_ui;
}

class fast_sheet_ui : public QWidget
{
    Q_OBJECT

public:
    enum n_style: int{
        line, /* draw only line */
        white, /*
                * in this case sheet drawing draws a single
                * line, at the end of the sheet, of the
                * same color as the sheet */
        empty, /* draw from default style in last style */
        square /* draw square */
    };

    explicit fast_sheet_ui(QWidget *parent = nullptr);
    ~fast_sheet_ui();

    bool auto_create = true;


    /* this function is call when we need to reload all style  */
    void needToReload();

    inline n_style WhatIsSelected(){ return this->m_how; }

private:
    void load();
    void save();

    style_struct *m_style;
    n_style m_how = n_style::empty;

    Ui::fast_sheet_ui *ui;

protected:
    bool event(QEvent *event) override;

private slots:
    void on_white_sheet_clicked();
};

#endif // FAST_SHEET_UI_H
