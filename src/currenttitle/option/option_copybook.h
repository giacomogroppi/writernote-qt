#ifndef OPTION_COPYBOOK_H
#define OPTION_COPYBOOK_H

#include "modality_data.h"

#include <QDialog>
#include <QList>

namespace Ui {
class option_copybook;
}

class option_copybook : public QDialog
{
    Q_OBJECT

public:
    explicit option_copybook(QWidget *parent = nullptr, QList<modality> *list = nullptr);
    ~option_copybook();

private:
    Ui::option_copybook *ui;
};

#endif // OPTION_COPYBOOK_H
