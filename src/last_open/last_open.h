#ifndef LAST_OPEN_H
#define LAST_OPEN_H

#include <QDialog>

#include <QList>

#include "element/element_ui.h"
#include "struct_last_file.h"
#include "../cloud/struct_user.h"
#include "../cloud/cloud_controll.h"

namespace Ui {
class last_open;
}

class last_open : public QDialog
{
    Q_OBJECT

public:
    void setDataReturn(char **data);

    explicit last_open(QWidget *parent = nullptr,
                       struct struct_user *user = nullptr,
                       cloud_controll *controll = nullptr);
    ~last_open();

    void setting_data(struct last_file *);

    int load_data_();

    struct struct_user *m_user;

private:
    cloud_controll *m_controll;

    void updateList();

    char **m_style_return;

    last_file *m_last;

    Ui::last_open *ui;

    QList<element_ui *> m_lista;

    void deleteIn(int);
    int m_quanti;
private slots:
    void on_clicked(int);
    void on_open_button_clicked();
    void deleteInElement(int);
    void downloadIn(int);
};

#endif // LAST_OPEN_H
