#ifndef CHOOSEPAGE_H
#define CHOOSEPAGE_H

#include "utils/common_script.h"
#include <QDialog>

namespace Ui {
class ChoosePage;
}

class ChoosePage : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosePage(QWidget *parent);
    ~ChoosePage();

    void show(const class Document &doc);
    QSize get_size() const;

private slots:
    void on_pushButton_apply_clicked();

    void on_pushButton_close_clicked();

    void on_plainTextEdit_textChanged();

    void on_pushButton_down_clicked();

    void on_pushButton_up_clicked();

private:
    int _curr = -1;
    Ui::ChoosePage *ui;
protected:
    bool event(QEvent *event) override;
};

force_inline QSize ChoosePage::get_size() const
{
    return this->size();
}

#endif // CHOOSEPAGE_H
