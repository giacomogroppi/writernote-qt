#ifndef CLOUD_UI_H
#define CLOUD_UI_H

#include <QDialog>
#include "../cloud/cloud_controll.h"

#define NAME "Cloud controll"
#define POSITION_LOG 1

namespace Ui {
class cloud_ui;
}

class cloud_ui : public QDialog
{
    Q_OBJECT

public:
    explicit cloud_ui(QDialog *parent = nullptr, cloud_controll *m_controll = nullptr);
    ~cloud_ui();

    Ui::cloud_ui *ui;

public slots:
    void on_tab_main_currentChanged(int index);
    void readyRead(QByteArray data, n_request::e_request);

private slots:
    void on_button_info_clicked();

    void on_close_button_clicked();

    void on_ac_1_clicked();

    void on_ac_3_clicked();

    void on_ac_6_clicked();

    void on_ac_12_clicked();


    void on_button_log_clicked();

    void on_button_register_clicked();

private:
    void serverDown();
    cloud_controll *m_controll;

    void refresh();

};

#endif // CLOUD_UI_H
