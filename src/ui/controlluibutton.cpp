#include "controlluibutton.h"
#include "mainwindow.h"
#include "ui/toolbar.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QSettings>
#include "utils/setting_define.h"

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

    this->_mode = static_cast<Mode>(val);
    setting.endGroup();

    this->initList();
}

void ControllUiButton::saveSettings()
{
    int val = (uint)_mode;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_MODALITY);
    setting.setValue(KEY_MODALITY, val);

    setting.endGroup();
}

void ControllUiButton::initList()
{
    /*Ui::MainWindow *ui = parent->ui;

    this->m_button.clear();

    m_button.append(ui->pushButton_back);
    m_button.append(ui->buttonStartRecording);
    m_button.append(ui->buttonStopRecording);
    m_button.append(ui->buttonPauseRecording);
    m_button.append(ui->buttonUndu);
    m_button.append(ui->buttonRedo);
    m_button.append(ui->buttonListen_current_audio);
    m_button.append(ui->buttonPen);
    m_button.append(ui->buttonRubber);
    m_button.append(ui->buttonselezionetext);
    m_button.append(ui->buttonInsertText);
    m_button.append(ui->buttonHighlighter);
    m_button.append(ui->buttonLaser);
    m_button.append(ui->buttonBlack);
    m_button.append(ui->buttonYellow);
    m_button.append(ui->buttonWhite);
    m_button.append(ui->buttonBrown);
    m_button.append(ui->buttonPurple);
    m_button.append(ui->buttonChooseColor);
    m_button.append(ui->buttonInsertImage);
    m_button.append(ui->buttonSheet);
    m_button.append(ui->buttonNewPage);
    m_button.append(ui->buttonRestore);
    m_button.append(ui->buttonPenOrMouse);
    m_button.append(ui->buttonRecentFile);
    m_button.append(ui->buttonChangeVisual);
    m_button.append(ui->buttonFullScreen);
    m_button.append(ui->buttonShowPreview);

    m_spacer.append(ui->horizontalSpacer_button);
    m_spacer.append(ui->horizontalSpacer_button1);
    m_spacer.append(ui->horizontalSpacer_button2);
    m_spacer.append(ui->horizontalSpacer_button3);*/
}

void ControllUiButton::update()
{
    //int i;
    //const int len_button = this->m_button.length();
    //const int len_spacer = this->m_spacer.length();
    //constexpr auto colorOver = "#e0e0e0";
    //const auto color = parent->ui->buttonChooseColor->palette().color(parent->backgroundRole());

    const bool tablet = this->_mode == Mode::Tablet;

    /*for(i = 0; i < len_button; i++){
        this->m_button.at(i)->setVisible(tablet);
        m_button.at(i)->setStyleSheet(qstr("QPushButton{border:5px solid %1;} QPushButton:hover{border:10px solid %2} QPushButton:checked{border:15px solid %2}").
                                                    arg(color.name()).arg(colorOver));
    }*/

    if(tablet){
        parent->_tool_bar->Show();
        parent->_tool_bar->setSpacer(QSizePolicy::MinimumExpanding);
    }else{
        parent->_tool_bar->Hide();
        parent->_tool_bar->setSpacer();
    }

#define vis_private setHidden
    parent->ui->mainbar->vis_private(tablet);
    parent->ui->simpleactionbar->vis_private(tablet);
    parent->ui->audiobar->vis_private(tablet);
    parent->ui->toolbarmatita->vis_private(tablet);
    parent->ui->toolBarcolore->vis_private(tablet);
    parent->ui->toolSheet->vis_private(tablet);
}

