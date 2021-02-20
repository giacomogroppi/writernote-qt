#ifndef ELEMENT_UI_H
#define ELEMENT_UI_H

#include <QWidget>
#include "../struct_last_file.h"

namespace Ui {
class element_ui;
}

class element_ui : public QWidget
{
    Q_OBJECT

public:
    explicit element_ui(QWidget *parent = nullptr);
    ~element_ui();

    void setData(last_file *, int index);

    int m_index = 0;

    void decrease();

private:
    void set_main();

    Ui::element_ui *ui;

    last_file *m_data = NULL;

signals:
    void on_pressed(int);
    void deleteIn(int);
    void downloadIn(int);

protected:
    bool event(QEvent *event) override;

private slots:
    void on_button_delete_clicked();
    void on_button_download_clicked();
};

#endif // ELEMENT_UI_H
