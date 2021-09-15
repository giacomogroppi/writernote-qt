#ifndef ELEMENT_UI_H
#define ELEMENT_UI_H

#include <QWidget>
#include <QListWidgetItem>
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

    void setData(last_file *, int index, QListWidgetItem *item);

    int m_index = 0;

    void decrease();
private:
    QListWidgetItem *item;
    void set_main(QListWidgetItem *item);

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
    void on_open_exe_clicked();
};

#endif // ELEMENT_UI_H
