#ifndef CLOUD_UI_H
#define CLOUD_UI_H

#include <QWidget>

namespace Ui {
class cloud_ui;
}

class cloud_ui : public QWidget
{
    Q_OBJECT

public:
    explicit cloud_ui(QWidget *parent = nullptr);
    ~cloud_ui();

private slots:
    void on_pushButton_clicked();

private:
    Ui::cloud_ui *ui;
};

#endif // CLOUD_UI_H
