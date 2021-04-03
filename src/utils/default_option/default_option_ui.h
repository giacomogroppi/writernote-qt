#ifndef DEFAULT_OPTION_UI_H
#define DEFAULT_OPTION_UI_H

#include <QDialog>

namespace Ui {
class default_option_ui;
}

class default_option_ui : public QDialog
{
    Q_OBJECT

public:
    explicit default_option_ui(QWidget *parent = nullptr);
    ~default_option_ui();

private:
    Ui::default_option_ui *ui;
};

#endif // DEFAULT_OPTION_UI_H
