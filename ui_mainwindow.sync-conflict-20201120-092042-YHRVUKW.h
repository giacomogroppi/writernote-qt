/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_File;
    QAction *actionSave_File;
    QAction *actionSave_As;
    QAction *actionPrint;
    QAction *actionRecord;
    QAction *actionImport_Video;
    QAction *actionStop_Record;
    QAction *actionImport_Video_2;
    QAction *actionUndu;
    QAction *actionRedo;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionSelect_all;
    QAction *actionPaste;
    QAction *actionSource_code;
    QAction *actionBug_report;
    QAction *actionDocumentation;
    QAction *actionListen_current_audio;
    QAction *actionDelete_copybook;
    QAction *actionCreate_new_copybook;
    QAction *actionConvert_audio_to_text;
    QAction *actionOpen;
    QAction *actionDelete_audio;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QListWidget *listWidgetSX;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *stopplaybotton;
    QPushButton *playbotton;
    QSlider *horizontalSlider_2;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox;
    QFontComboBox *fontComboBox;
    QToolButton *boldbotton;
    QToolButton *inserttablebotton;
    QToolButton *listbotton;
    QToolButton *insertimagebotton;
    QMenuBar *menubar;
    QMenu *menuWriternote;
    QMenu *menuEdit;
    QMenu *menuAudio_option;
    QMenu *menuHelp;
    QToolBar *mainbar;
    QToolBar *simpleactionbar;
    QToolBar *audiobar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1027, 671);
        actionNew_File = new QAction(MainWindow);
        actionNew_File->setObjectName(QString::fromUtf8("actionNew_File"));
        actionNew_File->setCheckable(false);
        actionNew_File->setChecked(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../images/newFileBig.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_File->setIcon(icon);
        actionSave_File = new QAction(MainWindow);
        actionSave_File->setObjectName(QString::fromUtf8("actionSave_File"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../images/disk.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_File->setIcon(icon1);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../images/disk--pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon2);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon3);
        actionRecord = new QAction(MainWindow);
        actionRecord->setObjectName(QString::fromUtf8("actionRecord"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("../images/recoding.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRecord->setIcon(icon4);
        actionImport_Video = new QAction(MainWindow);
        actionImport_Video->setObjectName(QString::fromUtf8("actionImport_Video"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("../images/importVideo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImport_Video->setIcon(icon5);
        actionStop_Record = new QAction(MainWindow);
        actionStop_Record->setObjectName(QString::fromUtf8("actionStop_Record"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../images/StopRecordingAudio.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop_Record->setIcon(icon6);
        actionImport_Video_2 = new QAction(MainWindow);
        actionImport_Video_2->setObjectName(QString::fromUtf8("actionImport_Video_2"));
        actionUndu = new QAction(MainWindow);
        actionUndu->setObjectName(QString::fromUtf8("actionUndu"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("../images/arrow-curve-180-left.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndu->setIcon(icon7);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("../images/arrow-curve.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon8);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8("../images/clipboard-paste-document-text.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon9);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8("../images/scissors.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon10);
        actionSelect_all = new QAction(MainWindow);
        actionSelect_all->setObjectName(QString::fromUtf8("actionSelect_all"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8("../images/selection-input.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect_all->setIcon(icon11);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionPaste->setIcon(icon9);
        actionSource_code = new QAction(MainWindow);
        actionSource_code->setObjectName(QString::fromUtf8("actionSource_code"));
        actionBug_report = new QAction(MainWindow);
        actionBug_report->setObjectName(QString::fromUtf8("actionBug_report"));
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName(QString::fromUtf8("actionDocumentation"));
        actionListen_current_audio = new QAction(MainWindow);
        actionListen_current_audio->setObjectName(QString::fromUtf8("actionListen_current_audio"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8("../images/manoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionListen_current_audio->setIcon(icon12);
        actionDelete_copybook = new QAction(MainWindow);
        actionDelete_copybook->setObjectName(QString::fromUtf8("actionDelete_copybook"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8("../images/deleteCopyBook.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete_copybook->setIcon(icon13);
        actionCreate_new_copybook = new QAction(MainWindow);
        actionCreate_new_copybook->setObjectName(QString::fromUtf8("actionCreate_new_copybook"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8("../images/newFile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreate_new_copybook->setIcon(icon14);
        actionConvert_audio_to_text = new QAction(MainWindow);
        actionConvert_audio_to_text->setObjectName(QString::fromUtf8("actionConvert_audio_to_text"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8("../images/text-speech.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConvert_audio_to_text->setIcon(icon15);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8("../images/blue-folder-open-document.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon16);
        actionDelete_audio = new QAction(MainWindow);
        actionDelete_audio->setObjectName(QString::fromUtf8("actionDelete_audio"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8("../images/deleteAudio.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete_audio->setIcon(icon17);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        listWidgetSX = new QListWidget(centralwidget);
        listWidgetSX->setObjectName(QString::fromUtf8("listWidgetSX"));
        listWidgetSX->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_4->addWidget(listWidgetSX);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        stopplaybotton = new QPushButton(centralwidget);
        stopplaybotton->setObjectName(QString::fromUtf8("stopplaybotton"));
        stopplaybotton->setMaximumSize(QSize(25, 25));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8("../images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopplaybotton->setIcon(icon18);

        horizontalLayout->addWidget(stopplaybotton);

        playbotton = new QPushButton(centralwidget);
        playbotton->setObjectName(QString::fromUtf8("playbotton"));
        playbotton->setMaximumSize(QSize(25, 25));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8("../images/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        playbotton->setIcon(icon19);

        horizontalLayout->addWidget(playbotton);

        horizontalSlider_2 = new QSlider(centralwidget);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_2);

        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_4->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_4);


        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        horizontalLayout_2->addWidget(spinBox);

        fontComboBox = new QFontComboBox(centralwidget);
        fontComboBox->setObjectName(QString::fromUtf8("fontComboBox"));

        horizontalLayout_2->addWidget(fontComboBox);

        boldbotton = new QToolButton(centralwidget);
        boldbotton->setObjectName(QString::fromUtf8("boldbotton"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8("../images/bold.png"), QSize(), QIcon::Normal, QIcon::Off);
        boldbotton->setIcon(icon20);

        horizontalLayout_2->addWidget(boldbotton);

        inserttablebotton = new QToolButton(centralwidget);
        inserttablebotton->setObjectName(QString::fromUtf8("inserttablebotton"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8("../images/table.png"), QSize(), QIcon::Normal, QIcon::Off);
        inserttablebotton->setIcon(icon21);

        horizontalLayout_2->addWidget(inserttablebotton);

        listbotton = new QToolButton(centralwidget);
        listbotton->setObjectName(QString::fromUtf8("listbotton"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8("../images/bullet.png"), QSize(), QIcon::Normal, QIcon::Off);
        listbotton->setIcon(icon22);

        horizontalLayout_2->addWidget(listbotton);

        insertimagebotton = new QToolButton(centralwidget);
        insertimagebotton->setObjectName(QString::fromUtf8("insertimagebotton"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8("../images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        insertimagebotton->setIcon(icon23);

        horizontalLayout_2->addWidget(insertimagebotton);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1027, 22));
        menuWriternote = new QMenu(menubar);
        menuWriternote->setObjectName(QString::fromUtf8("menuWriternote"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuAudio_option = new QMenu(menubar);
        menuAudio_option->setObjectName(QString::fromUtf8("menuAudio_option"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        mainbar = new QToolBar(MainWindow);
        mainbar->setObjectName(QString::fromUtf8("mainbar"));
        mainbar->setIconSize(QSize(20, 20));
        mainbar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainbar);
        simpleactionbar = new QToolBar(MainWindow);
        simpleactionbar->setObjectName(QString::fromUtf8("simpleactionbar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, simpleactionbar);
        audiobar = new QToolBar(MainWindow);
        audiobar->setObjectName(QString::fromUtf8("audiobar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, audiobar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuWriternote->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuAudio_option->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuWriternote->addAction(actionNew_File);
        menuWriternote->addAction(actionSave_File);
        menuWriternote->addAction(actionOpen);
        menuWriternote->addAction(actionSave_As);
        menuWriternote->addAction(actionPrint);
        menuWriternote->addAction(actionRecord);
        menuWriternote->addAction(actionStop_Record);
        menuWriternote->addAction(actionImport_Video);
        menuWriternote->addAction(actionImport_Video_2);
        menuEdit->addAction(actionUndu);
        menuEdit->addAction(actionRedo);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionSelect_all);
        menuAudio_option->addAction(actionListen_current_audio);
        menuAudio_option->addAction(actionCreate_new_copybook);
        menuAudio_option->addAction(actionDelete_audio);
        menuAudio_option->addAction(actionDelete_copybook);
        menuAudio_option->addAction(actionConvert_audio_to_text);
        menuHelp->addAction(actionSource_code);
        menuHelp->addAction(actionDocumentation);
        mainbar->addAction(actionNew_File);
        mainbar->addAction(actionOpen);
        mainbar->addAction(actionSave_File);
        mainbar->addAction(actionSave_As);
        mainbar->addAction(actionPrint);
        mainbar->addAction(actionRecord);
        mainbar->addAction(actionStop_Record);
        mainbar->addAction(actionImport_Video);
        simpleactionbar->addAction(actionRedo);
        simpleactionbar->addAction(actionCut);
        simpleactionbar->addAction(actionCopy);
        simpleactionbar->addAction(actionPaste);
        audiobar->addAction(actionListen_current_audio);
        audiobar->addAction(actionDelete_audio);
        audiobar->addAction(actionDelete_copybook);
        audiobar->addAction(actionCreate_new_copybook);
        audiobar->addAction(actionConvert_audio_to_text);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew_File->setText(QCoreApplication::translate("MainWindow", "New File", nullptr));
        actionSave_File->setText(QCoreApplication::translate("MainWindow", "Save File", nullptr));
        actionSave_As->setText(QCoreApplication::translate("MainWindow", "Save As...", nullptr));
        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print", nullptr));
        actionRecord->setText(QCoreApplication::translate("MainWindow", "Record", nullptr));
        actionImport_Video->setText(QCoreApplication::translate("MainWindow", "Import Video", nullptr));
        actionStop_Record->setText(QCoreApplication::translate("MainWindow", "Stop Record", nullptr));
        actionImport_Video_2->setText(QCoreApplication::translate("MainWindow", "Import Video", nullptr));
        actionUndu->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionSelect_all->setText(QCoreApplication::translate("MainWindow", "Select all", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionSource_code->setText(QCoreApplication::translate("MainWindow", "Source code", nullptr));
        actionBug_report->setText(QCoreApplication::translate("MainWindow", "Bug report", nullptr));
        actionDocumentation->setText(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
        actionListen_current_audio->setText(QCoreApplication::translate("MainWindow", "Listen current audio", nullptr));
        actionDelete_copybook->setText(QCoreApplication::translate("MainWindow", "Delete copybook", nullptr));
        actionCreate_new_copybook->setText(QCoreApplication::translate("MainWindow", "Create new copybook", nullptr));
        actionConvert_audio_to_text->setText(QCoreApplication::translate("MainWindow", "Convert audio to text", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open file", nullptr));
        actionDelete_audio->setText(QCoreApplication::translate("MainWindow", "Delete audio", nullptr));
        stopplaybotton->setText(QString());
        playbotton->setText(QString());
        boldbotton->setText(QString());
        inserttablebotton->setText(QString());
        listbotton->setText(QString());
        insertimagebotton->setText(QString());
        menuWriternote->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuAudio_option->setTitle(QCoreApplication::translate("MainWindow", "Audio option", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        mainbar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        simpleactionbar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
        audiobar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
