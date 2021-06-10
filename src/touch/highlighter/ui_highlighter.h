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

private slots:
    void on_slider_actionTriggered(int action);

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
