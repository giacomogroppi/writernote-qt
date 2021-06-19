#ifndef UI_OPTION_COPYBOOK_H
#define UI_OPTION_COPYBOOK_H

#include "modality_data.h"

#include <QDialog>
#include <QList>

namespace Ui {
class ui_option_copybook;
}

class ui_option_copybook : public QDialog
{
    Q_OBJECT

public:
    explicit ui_option_copybook(QWidget *parent = nullptr, QList<modality> *list = nullptr);
    ~ui_option_copybook();

private:
    Ui::ui_option_copybook *ui;
};

#endif // UI_OPTION_COPYBOOK_H
