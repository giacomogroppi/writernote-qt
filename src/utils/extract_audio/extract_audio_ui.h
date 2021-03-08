#ifndef EXTRACT_AUDIO_UI_H
#define EXTRACT_AUDIO_UI_H

#include <QDialog>
#include "../../indice_class.h"
#include "../../currenttitle/currenttitle_class.h"

namespace Ui {
class extract_audio_ui;
}

class extract_audio_ui : public QDialog
{
    Q_OBJECT

public:
    explicit extract_audio_ui(QWidget *parent = nullptr,
                              indice_class *indice = nullptr,
                              QString *path = nullptr);
    ~extract_audio_ui();

private slots:
    void on_button_close_clicked();

    void on_extract_to_clicked();

    void on_edit_path_textChanged();

private:

    void ChangeColor(QColor);

    /*
     * iswriting if true if the user is writing in path
    */
    bool iswriting = false;

    void setData();

    /*
     * true if we create it,
     * false if the system pass
     *
     * it's use for delete
    */
    bool ext = false;
    indice_class *m_indice = nullptr;
    currenttitle_class *m_current = nullptr;

    Ui::extract_audio_ui *ui;

protected:
    bool event(QEvent *) override;

};

#endif // EXTRACT_AUDIO_UI_H
