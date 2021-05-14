#ifndef UI_RESTORE_H
#define UI_RESTORE_H

#include <QDialog>
#include <QString>
#include <QStringList>

namespace Ui {
class ui_restore;
}

class ui_restore : public QDialog
{
    Q_OBJECT

public:
    explicit ui_restore(QWidget *parent = nullptr,
                        QString path = "");
    ~ui_restore();

private:
    QStringList __l;
    QString path;
    Ui::ui_restore *ui;
};

#endif // UI_RESTORE_H
