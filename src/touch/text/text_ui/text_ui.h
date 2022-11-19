#pragma once

#include <QWidget>
#include <QColor>
#include <QFont>

#include "touch/dataTouch/datastruct/datastruct.h"
#define D_SIZE 8

namespace Ui {
class text_ui;
}

class text_ui : public QWidget
{
    Q_OBJECT

public:
    explicit text_ui(QWidget *parent = nullptr);
    ~text_ui();

    colore_s m_currentDcolor = colore_s(0, 0, 255, 255);
    int m_currentSize = D_SIZE;
    QFont m_font;

    void loadData();
    void saveData();

private:
    Ui::text_ui *ui;
    void drawButton();

protected:
    bool event(QEvent *event) override;
private slots:
    void on_button_color_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_fontComboBox_currentFontChanged(const QFont &f);
};

