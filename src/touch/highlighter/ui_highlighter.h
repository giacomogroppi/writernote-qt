#ifndef UI_HIGHLIGHTER_H
#define UI_HIGHLIGHTER_H

#include <QDialog>

namespace Ui {
class ui_highlighter;
}

class ui_highlighter : public QDialog
{
    Q_OBJECT

public:
    explicit ui_highlighter(QWidget *parent = nullptr);
    ~ui_highlighter();

    inline uchar getAlfa(){
        return m_data.alfa;
    }

private:
    struct data{
        uchar alfa;
    };

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::ui_highlighter *ui;
};

#endif // UI_HIGHLIGHTER_H
