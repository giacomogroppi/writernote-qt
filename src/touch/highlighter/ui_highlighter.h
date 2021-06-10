#ifndef UI_HIGHLIGHTER_H
#define UI_HIGHLIGHTER_H

#include <QDialog>

namespace Ui {
class ui_highlighter;
}

class pen_ui;

class ui_highlighter : public QDialog
{
    Q_OBJECT

public:
    explicit ui_highlighter(QWidget *parent = nullptr, bool *same = nullptr, pen_ui * par = nullptr);
    ~ui_highlighter();

    inline uchar getAlfa(){
        return m_data.alfa;
    }

    bool *same_data = nullptr;
    pen_ui *m_pen = nullptr;

private slots:
    void on_slider_actionTriggered(int action);

    void on_checkBox_stateChanged(int arg1);

private:
    struct data{
        uchar alfa;
    };

    void updateList();

    struct data m_data;

    void loadSettings();
    void saveSettings();

    Ui::ui_highlighter *ui;

protected:
    bool event(QEvent *) override;
};

#endif // UI_HIGHLIGHTER_H
