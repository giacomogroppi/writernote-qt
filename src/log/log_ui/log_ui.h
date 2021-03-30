#ifndef LOG_UI_H
#define LOG_UI_H

#include <QWidget>

namespace Ui {
class log_ui;
}

class log_ui : public QWidget
{
    Q_OBJECT

public:
    explicit log_ui(QWidget *parent = nullptr);
    ~log_ui();

private:
    Ui::log_ui *ui;
};

#endif // LOG_UI_H
