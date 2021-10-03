#include "controlluibutton.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QSettings>
#include "../utils/setting_define.h"

ControllUiButton::ControllUiButton(MainWindow *parent)
{
    this->parent = parent;
    this->loadSettings();
    this->initList();
    this->update();
}

ControllUiButton::~ControllUiButton()
{
    this->saveSettings();
}

void ControllUiButton::drawImageButton(const QString &path, QPushButton *button, const QSize &size)
{
    QPixmap pix(path);
    QIcon icon(pix);
    button->setIcon(icon);
    button->setIconSize(size);
}

void ControllUiButton::loadSettings()
{
    int val;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_MODALITY);
    val = setting.value(KEY_MODALITY, (uint)Mode::Computer).toUInt();

    this->mode = static_cast<Mode>(val);
    setting.endGroup();

    this->initList();
}

void ControllUiButton::saveSettings()
{
    int val = (uint)mode;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_MODALITY);
    setting.setValue(KEY_MODALITY, val);

    setting.endGroup();
}

void ControllUiButton::initList()
{
    Ui::MainWindow *ui = parent->ui;
    this->m_button.clear();
    this->m_button.append(ui->pushButton_back);
}

