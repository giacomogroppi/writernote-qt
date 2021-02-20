#ifndef TEXT_UI_H
#define TEXT_UI_H

#include <QWidget>

namespace Ui {
class text_ui;
}

class text_ui : public QWidget
{
    Q_OBJECT

public:
    explicit text_ui(QWidget *parent = nullptr);
    ~text_ui();

private:
    Ui::text_ui *ui;
};

#endif // TEXT_UI_H
