#ifndef PROGRESS_BAR_UI_H
#define PROGRESS_BAR_UI_H

#include <QDialog>

namespace Ui {
class progress_bar_ui;
}

class progress_bar_ui : public QDialog
{
    Q_OBJECT

public:
    explicit progress_bar_ui(QWidget *parent = nullptr,
                             const char *title = nullptr,
                             const char *body = nullptr);
    ~progress_bar_ui();

public slots:
    void finished_();
    void progress_(int);

private slots:
    void on_close_button_clicked();

private:
    Ui::progress_bar_ui *ui;

signals:
    void closeForce();
};

#endif // PROGRESS_BAR_UI_H
