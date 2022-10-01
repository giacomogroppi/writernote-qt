#pragma once

#include <QDialog>
#include <QString>
#include "datawrite/savefile.h"
#include "dataread/xmlstruct.h"
#include "currenttitle/document.h"

namespace Ui {
class restore;
}

class restore : public QDialog
{
    Q_OBJECT

public:
    explicit restore(QWidget *parent,
                     QByteArray path);
    ~restore();

private slots:
    void on_close_button_clicked();

    void on_pushButton_open_clicked();

    void on_ok_restore_clicked();

private:
    static void removeNotWriternote(QList<QString> &l);
    void updateList();

    savefile *m_save = nullptr;
    xmlstruct *m_xml = nullptr;
    Document *m_curr;

    QList<QString> __l;

    QByteArray path;

    Ui::restore *ui;
};
