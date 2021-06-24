#ifndef ARE_YOU_SURE_CPP
#define ARE_YOU_SURE_CPP

#include <QString>
#include <QDialog>
#include <QMessageBox>

static inline bool areyousure(const QString &title, const QString body){
    QMessageBox::StandardButton resBtn = QMessageBox::question( nullptr, title,
                                                                body,
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes)
        return true;

    return false;
}

#endif
