#pragma once


#include <QString>
#include <QList>
#include <QPushButton>
#include <QColor>
#include <QImage>
#include <QSpacerItem>
#include "utils/common_script.h"

class MainWindow;

class ControllUiButton
{
private:
    MainWindow *parent;
    //QList<QPushButton *> m_button;
    //QList<QSpacerItem *> m_spacer;

public:
    ControllUiButton(MainWindow *parent);
    ~ControllUiButton();
    static void drawImageButton(const QString &path, QPushButton *button, const QSize &size);

    enum Mode{
        Tablet,
        Computer
    };

    force_inline void setTo(const Mode mode)
    {
        this->_mode = mode;
        this->update();
    }

    force_inline void change()
    {
        this->_mode = (this->_mode == Mode::Tablet) ? Mode::Computer : Mode::Tablet;
        this->update();
    }

private :
    enum Mode _mode;
    void loadSettings();
    void saveSettings();

    void initList();
    void update();

    static QColor get_color_background();
};

force_inline QColor ControllUiButton::get_color_background()
{
    return QColor(224, 224, 224);
}

