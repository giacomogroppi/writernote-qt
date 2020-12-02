/********************************************************************************
** Form generated from reading UI file 'audioqualityoption.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOQUALITYOPTION_H
#define UI_AUDIOQUALITYOPTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_audioqualityoption
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QComboBox *audioDeviceBox;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QComboBox *containerBox;
    QComboBox *sampleRateBox;
    QComboBox *audioCodecBox;
    QLabel *label_5;
    QComboBox *channelsBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QSlider *qualitySlider;
    QRadioButton *constantQualityRadioButton;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *cancelbutton;
    QStatusBar *statusbar;

    void setupUi(QDialog *audioqualityoption)
    {
        if (audioqualityoption->objectName().isEmpty())
            audioqualityoption->setObjectName(QString::fromUtf8("audioqualityoption"));
        audioqualityoption->resize(241, 342);
        centralwidget = new QWidget(audioqualityoption);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(10, 10, 221, 321));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        audioDeviceBox = new QComboBox(centralwidget);
        audioDeviceBox->setObjectName(QString::fromUtf8("audioDeviceBox"));

        gridLayout_2->addWidget(audioDeviceBox, 0, 1, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        containerBox = new QComboBox(centralwidget);
        containerBox->setObjectName(QString::fromUtf8("containerBox"));

        gridLayout_2->addWidget(containerBox, 2, 1, 1, 1);

        sampleRateBox = new QComboBox(centralwidget);
        sampleRateBox->setObjectName(QString::fromUtf8("sampleRateBox"));

        gridLayout_2->addWidget(sampleRateBox, 3, 1, 1, 1);

        audioCodecBox = new QComboBox(centralwidget);
        audioCodecBox->setObjectName(QString::fromUtf8("audioCodecBox"));

        gridLayout_2->addWidget(audioCodecBox, 1, 1, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        channelsBox = new QComboBox(centralwidget);
        channelsBox->setObjectName(QString::fromUtf8("channelsBox"));

        gridLayout_2->addWidget(channelsBox, 4, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 3);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        qualitySlider = new QSlider(groupBox);
        qualitySlider->setObjectName(QString::fromUtf8("qualitySlider"));
        qualitySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(qualitySlider, 1, 1, 1, 1);

        constantQualityRadioButton = new QRadioButton(groupBox);
        constantQualityRadioButton->setObjectName(QString::fromUtf8("constantQualityRadioButton"));
        constantQualityRadioButton->setChecked(true);

        gridLayout->addWidget(constantQualityRadioButton, 0, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        cancelbutton = new QPushButton(centralwidget);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));

        horizontalLayout->addWidget(cancelbutton);


        gridLayout_3->addLayout(horizontalLayout, 2, 0, 1, 3);

        statusbar = new QStatusBar(audioqualityoption);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 0, 3, 22));

        retranslateUi(audioqualityoption);
        QObject::connect(constantQualityRadioButton, SIGNAL(toggled(bool)), qualitySlider, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(audioqualityoption);
    } // setupUi

    void retranslateUi(QDialog *audioqualityoption)
    {
        audioqualityoption->setWindowTitle(QCoreApplication::translate("audioqualityoption", "MainWindow", nullptr));
        label_4->setText(QCoreApplication::translate("audioqualityoption", "Sample rate:", nullptr));
        label_2->setText(QCoreApplication::translate("audioqualityoption", "Audio Codec:", nullptr));
        label->setText(QCoreApplication::translate("audioqualityoption", "Input Device:", nullptr));
        label_3->setText(QCoreApplication::translate("audioqualityoption", "File Container:", nullptr));
        label_5->setText(QCoreApplication::translate("audioqualityoption", "Channels:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("audioqualityoption", "Encoding Mode:", nullptr));
        constantQualityRadioButton->setText(QCoreApplication::translate("audioqualityoption", "Constant Quality:", nullptr));
        pushButton->setText(QCoreApplication::translate("audioqualityoption", "Ok", nullptr));
        cancelbutton->setText(QCoreApplication::translate("audioqualityoption", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class audioqualityoption: public Ui_audioqualityoption {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOQUALITYOPTION_H
