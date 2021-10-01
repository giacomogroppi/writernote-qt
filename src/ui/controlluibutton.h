#ifndef CONTROLLUIBUTTON_H
#define CONTROLLUIBUTTON_H
class MainWindow;
#include <QString>
#include <QList>
#include <QPushButton>

class ControllUiButton
{
private:
    MainWindow *parent;
    QList<QPushButton *> m_button;

    void initList();

public:
    ControllUiButton(MainWindow *parent);

};

#endif // CONTROLLUIBUTTON_H
