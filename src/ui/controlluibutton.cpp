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

void ControllUiButton::update()
{
    int i, len;
    const bool tablet = this->mode == Mode::Tablet;

    parent->ui->actionAbout_Qt->setVisible(!tablet);
    parent->ui->actionBug_report->setVisible(!tablet);
    parent->ui->actionChange_language->setVisible(!tablet);
    parent->ui->actionCheck_update->setVisible(!tablet);
    parent->ui->actionCopy->setVisible(!tablet);
    parent->ui->actionCopybook->setVisible(!tablet);
    parent->ui->actionCut->setVisible(!tablet);
    parent->ui->actionDefault_method_write->setVisible(!tablet);
    parent->ui->actionDefault_save_location->setVisible(!tablet);
    parent->ui->actionDelete_audio->setVisible(!tablet);
    parent->ui->actionDocumentation->setVisible(!tablet);
    parent->ui->actionEnable_redo_undo->setVisible(!tablet);
    parent->ui->actionExtract_audio->setVisible(!tablet);
    parent->ui->actionNew_File->setVisible(!tablet);
    parent->ui->actionRecent_file->setVisible(!tablet);
    parent->ui->actionRestore_file->setVisible(!tablet);
    parent->ui->actionhighlighter->setVisible(!tablet);
    parent->ui->actioninsertImage->setVisible(!tablet);
    parent->ui->actionImport_Video->setVisible(!tablet);
    parent->ui->actioninsertText->setVisible(!tablet);

    parent->ui->pushButton_back->setHidden(!tablet);
}

