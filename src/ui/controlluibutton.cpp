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
    const bool tablet = this->mode == Mode::Tablet;
    //const bool tablet = true;

    parent->ui->mainbar->setHidden(tablet);
    parent->ui->simpleactionbar->setHidden(tablet);
    parent->ui->audiobar->setHidden(tablet);
    parent->ui->toolbarmatita->setHidden(tablet);
    parent->ui->toolBarcolore->setHidden(tablet);
    parent->ui->toolSheet->setHidden(tablet);

    parent->ui->pushButton_back->setHidden(!tablet);
    parent->ui->buttonStartRecording->setHidden(!tablet);
    parent->ui->buttonStopRecording->setHidden(!tablet);
    parent->ui->buttonPauseRecording->setHidden(!tablet);
    parent->ui->buttonUndu->setHidden(!tablet);
    parent->ui->buttonRedo->setHidden(!tablet);
    parent->ui->buttonListen_current_audio->setHidden(!tablet);
    parent->ui->buttonPen->setHidden(!tablet);
    parent->ui->buttonRubber->setHidden(!tablet);
    parent->ui->buttonselezionetext->setHidden(!tablet);
    parent->ui->buttonInsertText->setHidden(!tablet);
    parent->ui->buttonHighlighter->setHidden(!tablet);
    parent->ui->buttonBlack->setHidden(!tablet);
    parent->ui->buttonYellow->setHidden(!tablet);
    parent->ui->buttonWhite->setHidden(!tablet);
    parent->ui->buttonBrown->setHidden(!tablet);
    parent->ui->buttonPurple->setHidden(!tablet);
    parent->ui->buttonChooseColor->setHidden(!tablet);
    parent->ui->buttonInsertImage->setHidden(!tablet);
    parent->ui->buttonSheet->setHidden(!tablet);
    parent->ui->buttonNewPage->setHidden(!tablet);
    parent->ui->buttonRestore->setHidden(!tablet);
    parent->ui->buttonPenOrMouse->setHidden(!tablet);
    parent->ui->buttonRecentFile->setHidden(!tablet);
    parent->ui->buttonChangeVisual->setHidden(!tablet);
}

