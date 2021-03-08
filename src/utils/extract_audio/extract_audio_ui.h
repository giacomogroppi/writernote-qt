#ifndef EXTRACT_AUDIO_UI_H
#define EXTRACT_AUDIO_UI_H

#include <QDialog>

namespace Ui {
class extract_audio_ui;
}

class extract_audio_ui : public QDialog
{
    Q_OBJECT

public:
    explicit extract_audio_ui(QWidget *parent = nullptr);
    ~extract_audio_ui();

private:
    Ui::extract_audio_ui *ui;
};

#endif // EXTRACT_AUDIO_UI_H
