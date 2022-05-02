#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

namespace Ui {
class ToolBar;
}

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar();

    void Hide();
    void Show();

    void setSpacer(QSizePolicy::Policy hData = QSizePolicy::Minimum);

private slots:
    void on_button_back_clicked();
    void on_button_recent_file_clicked();
    void on_button_pen_clicked();
    void on_button_rubber_clicked();
    void on_button_cut_clicked();
    void on_button_text_clicked();
    void on_button_highlighter_clicked();
    void on_button_laser_clicked();
    void on_button_color_black_clicked();
    void on_button_color_white_clicked();
    void on_button_color_yellow_clicked();
    void on_button_color_brown_clicked();
    void on_button_color_purple_clicked();
    void on_button_choose_color_clicked();
    void on_button_insert_image_clicked();
    void on_button_new_page_clicked();
    void on_button_restore_clicked();
    void on_button_pen_or_mouse_clicked();
    void on_button_tablet_view_clicked();
    void on_button_full_screen_clicked();
    void on_button_hide_preview_clicked();
    void on_button_rec_start_clicked();
    void on_button_rec_stop_clicked();
    void on_button_rec_pause_clicked();
    void on_button_replay_clicked();
    void on_button_undo_clicked();
    void on_button_redo_clicked();

public:
    Ui::ToolBar *ui;
};

#endif // TOOLBAR_H
