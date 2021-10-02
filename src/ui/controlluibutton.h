#ifndef CONTROLLUIBUTTON_H
#define CONTROLLUIBUTTON_H
class MainWindow;
#include <QString>
#include <QList>
#include <QPushButton>
#include <QColor>
#include <QImage>

class ControllUiButton
{
private:
    MainWindow *parent;
    QList<QPushButton *> m_button;



public:
    ControllUiButton(MainWindow *parent);
    ~ControllUiButton();
    static void drawImageButton(const QString &path, QPushButton *button, const QSize &size);

    enum Mode: uchar{
        Tablet,
        Computer
    };

private :
    enum Mode mode;
    void loadSettings();
    void saveSettings();

    void initList();
    void update();
};

#endif // CONTROLLUIBUTTON_H
