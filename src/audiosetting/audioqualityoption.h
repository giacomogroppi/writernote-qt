#ifndef AUDIOQUALITYOPTION_H
#define AUDIOQUALITYOPTION_H

#include "../mainwindow.h"


#include <QDialog>

namespace Ui {
class audioqualityoption;
}

class audioqualityoption : public QDialog
{
    Q_OBJECT

public:
    Ui::audioqualityoption *ui;
    MainWindow *padre;

    QList<AudioLevel*> m_audioLevels;
    bool m_outputLocationSet = false;

    explicit audioqualityoption(QWidget *parent = nullptr, MainWindow *padre = nullptr);
    ~audioqualityoption();

private slots:
    void on_pushButton_2_clicked();

    void on_cancelbutton_clicked();

    void on_pushButton_clicked();

private:




};

#endif // AUDIOQUALITYOPTION_H
