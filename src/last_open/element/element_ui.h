#ifndef ELEMENT_UI_H
#define ELEMENT_UI_H

#include <QWidget>
#include <QListWidgetItem>
#include "../struct_last_file.h"
#include "imageWidget/imageWidget.h"

namespace Ui {
class element_ui;
}

class element_ui : public QWidget
{
    Q_OBJECT

public:
    explicit element_ui(QWidget *parent = nullptr, const last_file_s *data = nullptr, const bool showOnlyName = false, int m_index = -1);
    ~element_ui();

    void decrease();
    void showOnlyname(const bool showOnlyName);
private:
    void set_main();
    Ui::element_ui *ui;
    const last_file_s *m_data = NULL;
    imageWidget *img_widget;
    int m_index;
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
