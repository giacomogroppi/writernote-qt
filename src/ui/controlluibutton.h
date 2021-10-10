#ifndef CONTROLLUIBUTTON_H
#define CONTROLLUIBUTTON_H
class MainWindow;
#include <QString>
#include <QList>
#include <QPushButton>
#include <QColor>
#include <QImage>
#include <QSpacerItem>

class ControllUiButton
{
private:
    MainWindow *parent;
    QList<QPushButton *> m_button;
    QList<QSpacerItem *> m_spacer;

public:
    ControllUiButton(MainWindow *parent);
    ~ControllUiButton();
    static void drawImageButton(const QString &path, QPushButton *button, const QSize &size);

    enum Mode: uchar{
        Tablet,
        Computer
    };

    void change(){
        this->mode = (this->mode == Mode::Tablet) ? Mode::Computer : Mode::Tablet;
        this->update();
    }

private :
    enum Mode mode;
    void loadSettings();
    void saveSettings();

    void initList();
    void update();
};

#endif // CONTROLLUIBUTTON_H
