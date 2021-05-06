#ifndef UPDATER_H
#define UPDATER_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class updater; }
QT_END_NAMESPACE

class updater : public QMainWindow
{
    Q_OBJECT

public:
    updater(QWidget *parent = nullptr);
    ~updater();

    bool downloadFile(QString &l);
    bool exstractFile(QString &l, QString &dest);

private:
    Ui::updater *ui;
};
#endif // UPDATER_H
