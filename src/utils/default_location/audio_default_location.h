#ifndef AUDIO_DEFAULT_LOCATION_H
#define AUDIO_DEFAULT_LOCATION_H

#include <QDialog>

namespace Ui {
class audio_default_location;
}

namespace audio_default {
    enum n_audio_recod{
        internal,
        external
    };
    n_audio_recod load_default();
};

class audio_default_location : public QDialog
{
    Q_OBJECT

public:
    explicit audio_default_location(QWidget *parent = nullptr);
    ~audio_default_location();

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

    void on_button_ext_clicked();

    void on_button_into_clicked();

private:
    Ui::audio_default_location *ui;

    void save_data();
};

#endif // AUDIO_DEFAULT_LOCATION_H
