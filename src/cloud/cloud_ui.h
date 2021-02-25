#ifndef CLOUD_UI_H
#define CLOUD_UI_H

#include <QDialog>

namespace Ui {
class cloud_ui;
}

class cloud_ui : public QDialog
{
    Q_OBJECT

public:
    explicit cloud_ui(QDialog *parent = nullptr);
    ~cloud_ui();

private slots:
    void on_button_info_clicked();

    void on_close_button_clicked();

    void on_ac_1_clicked();

    void on_ac_3_clicked();

    void on_ac_6_clicked();

    void on_ac_12_clicked();

private:
    Ui::cloud_ui *ui;
};

#endif // CLOUD_UI_H
