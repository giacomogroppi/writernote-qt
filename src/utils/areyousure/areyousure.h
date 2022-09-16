#pragma once


#include <QString>
#include <QDialog>
#include <QMessageBox>

inline bool areyousure(const QString &title, const QString &body)
{
    QMessageBox::StandardButton resBtn;
    resBtn = QMessageBox::question( nullptr, title, body,
                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes)
        return true;

    return false;
}

