#ifndef FAST_SHEET_UI_H
#define FAST_SHEET_UI_H

#include <QWidget>
#include "../style_struct.h"
#include <QEvent>
#include <QListWidgetItem>

namespace Ui {
class fast_sheet_ui;
}

enum n_style: int{
    line, /* draw only line */
    white, /*
            * in this case sheet drawing draws a single
            * line, at the end of the sheet, of the
            * same color as the sheet */
    empty, /* draw from default style in last style */
    square /* draw square */
};

class fast_sheet_ui : public QWidget
{
    Q_OBJECT

public:
    explicit fast_sheet_ui(QWidget *parent = nullptr);
    ~fast_sheet_ui();

    bool auto_create = true;


    /* this function is call when we need to reload all style  */
    void needToReload();

    inline n_style WhatIsSelected(){ return this->m_how; }

private:
    void load();
    void save();

    style_struct m_style;
    n_style m_how = n_style::empty;

    Ui::fast_sheet_ui *ui;

    void updateCheck();

protected:
    bool event(QEvent *event) override;

private slots:
    void on_white_sheet_clicked();
    void on_lines_sheet_clicked();
    void on_shared_sheet_clicked();
    void on_list_sheet_itemClicked(QListWidgetItem *item);
    void on_autocreate_sheet_clicked();

signals:
    /* if res == true hide new page button */
    void changeButton(bool res);

};

#endif // FAST_SHEET_UI_H
