#pragma once


#include <QDialog>

namespace Ui {
class audio_default_location;
}


class audio_default_location : public QDialog
{
    Q_OBJECT

public:
    enum n_audio_record{
        internal,
        external,
        not_define
    };
    static n_audio_record load_default();

    explicit audio_default_location(QWidget *parent = nullptr);
    ~audio_default_location();

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

    void on_button_ext_clicked();

    void on_button_into_clicked();

private:
    n_audio_record m_lastchoise = n_audio_record::not_define;

    Ui::audio_default_location *ui;

    void save_data();
};
