#ifndef LAST_OPEN_H
#define LAST_OPEN_H

#include <QDialog>

#define MAXSTR 100

#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

#include <QList>

struct last_file{
    char posizione[MAXSTR];
    short int type;
};

namespace Ui {
class last_open;
}

class last_open : public QDialog
{
    Q_OBJECT

public:
    explicit last_open(QWidget *parent = nullptr);
    ~last_open();

    void setting_data(struct last_file *);

    int load_data();
private:
    struct last_file *m_last_file;

    Ui::last_open *ui;


};

#endif // LAST_OPEN_H
