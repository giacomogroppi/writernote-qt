#ifndef RUBBER_UI_H
#define RUBBER_UI_H

#include <QWidget>

#define TOTALE 1
#define PARZIALE 2

#define DEFAULT_GOMMA_SIZE 5

namespace Ui {
class rubber_ui;
}

class rubber_ui : public QWidget
{
    Q_OBJECT

public:
    explicit rubber_ui(QWidget *parent = nullptr);
    ~rubber_ui();

    void save_settings();
    void load_settings();

    int m_size_gomma = DEFAULT_GOMMA_SIZE;
    int m_type_gomma = TOTALE;

private:
    void update_data();

    Ui::rubber_ui *ui;

protected:
    bool event(QEvent *) override;
private slots:
    void on_totale_button_clicked();
    void on_partial_button_clicked();
};

#endif // RUBBER_UI_H
