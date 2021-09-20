#ifndef EXTRACT_AUDIO_UI_H
#define EXTRACT_AUDIO_UI_H

#include <QDialog>
#include "../../indice_class.h"
#include "../../currenttitle/document.h"

namespace Ui {
class extract_audio_ui;
}

class extract_audio_ui : public QDialog
{
    Q_OBJECT

public:
    explicit extract_audio_ui(QWidget *parent,
                              const QString *path);
    ~extract_audio_ui();

private slots:
    void on_button_close_clicked();

    void on_extract_to_clicked();

    void on_edit_path_textChanged();

    void on_open_to_clicked();

    void on_open_from_clicked();

private:

    void ChangeColor(QColor);

    /*
     * iswriting if true if the user is writing in path
    */
    bool iswriting = false;

    /*
     * true if we create it,
     * false if the system pass
     *
     * it's use for delete
    */
    bool ext = false;
    Document *m_current = nullptr;

    Ui::extract_audio_ui *ui;

protected:
    bool event(QEvent *) override;

};

#endif // EXTRACT_AUDIO_UI_H
