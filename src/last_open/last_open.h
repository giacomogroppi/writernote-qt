#ifndef LAST_OPEN_H
#define LAST_OPEN_H

#include <QDialog>

#include <QList>

#include "element/element_ui.h"
#include "struct_last_file.h"


namespace Ui {
class last_open;
}

class last_open : public QDialog
{
    Q_OBJECT

public:
    void setDataReturn(last_file **data);

    explicit last_open(QWidget *parent = nullptr);
    ~last_open();

    void setting_data(struct last_file *);

    int load_data_();

private:
    void updateList();

    last_file **m_style_return;

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
