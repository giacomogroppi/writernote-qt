#ifndef LAST_OPEN_H
#define LAST_OPEN_H

#include <QDialog>

#include <QList>
#include <QListWidgetItem>
#include "element/element_ui.h"
#include "struct_last_file.h"
#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"
#include "widget_parent/widget_parent.h"

namespace Ui {
class last_open;
}

class last_open : public QDialog
{
    Q_OBJECT

public:
    void setDataReturn(char **data);
    enum Method: uchar{
        OpenFile, /* for android -> last_open will open the folder where all the file are store, and ask the user when he want to save the file */
        OpenRecent /* for android and desktop system */
    };
    explicit last_open(QWidget *parent,
                       struct struct_user *user,
                       cloud_controll *controll,
                       bool *m_closeall,
                       Method method);
    ~last_open();

    void setting_data(struct last_file_s *);

    int load_data_();

    struct struct_user *m_user;

private:
    void loadGeometry();
    Method m_currentMethod;
    widget_parent *m_parent = nullptr;
    /* open file */
    uchar on_click_ex(const char *pos);

    cloud_controll *m_controll;

    void updateList();

    char **m_style_return;

    last_file m_last;

    Ui::last_open *ui;

    void deleteIn(int);
    bool *m_closeall;
public slots:
    void on_clicked(int);
    void on_open_button_clicked();
    void deleteInElement(int);
    void downloadIn(int);
    void on_close_all_clicked();
    void on_close_button_clicked();
    void on_option_button_clicked();
};

#endif // LAST_OPEN_H
