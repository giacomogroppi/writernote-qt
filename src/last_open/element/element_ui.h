#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "last_open/struct_last_file.h"
#include "imageWidget/imageWidget.h"

namespace Ui {
class element_ui;
}

class element_ui : public QWidget
{
    Q_OBJECT

public:
    explicit element_ui(QWidget *parent, const last_file_s *data,
                        cbool showOnlyName, int m_index, const uchar __showFileOnlyIfExist);
    ~element_ui();

    void decrease();
    void showOnlyname(cbool showOnlyName);
    int needToDelete;
private:
    void set_main();
    Ui::element_ui *ui;
    const last_file_s *m_data = NULL;
    imageWidget *img_widget;
    int m_index;
    uchar __showFileOnlyIfExist;
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

