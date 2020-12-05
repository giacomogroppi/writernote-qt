#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "../mainwindow.h"

namespace Ui {
class videocompress_ui;
}

class videocompress_ui : public QDialog
{
    Q_OBJECT

public:
    explicit videocompress_ui(QWidget *parent = nullptr, MainWindow *padre = nullptr);
    ~videocompress_ui();

    MainWindow *padre;
    Ui::videocompress_ui *ui;

    QString posizionefrom;
    QString posizioneto;

private slots:
    void on_pushButton_4_clicked();

    void on_plainTextEdit_from_textChanged();

    //void on_bottonedestinazione_textChanged();

    void on_textedit_destinazione_textChanged();

    void on_pushButton_clicked();

    void on_bottonefrom_clicked();

    void on_pushButton_ok_clicked();

private:
};

#endif // DIALOG_H
