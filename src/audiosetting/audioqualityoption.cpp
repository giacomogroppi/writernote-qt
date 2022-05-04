#include "audioqualityoption.h"
#include "ui_audioqualityoption.h"
#include "mainwindow.h"
#include "audiorecord/audiorecord.h"
#include "savequalita.h"
#include "utils/dialog_critic/dialog_critic.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
audioqualityoption::audioqualityoption(QWidget *parent, MainWindow *padre) :
    QDialog(parent),
    ui(new Ui::audioqualityoption)
{
    ui->setupUi(this);
    this->padre = padre;
    QAudioRecorder *recorder = padre->m_audio_recorder->recorder;

    //audio devices
    ui->audioDeviceBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &device: recorder->audioInputs()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }

    //audio codecs
    ui->audioCodecBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &codecName: recorder->supportedAudioCodecs()) {
        ui->audioCodecBox->addItem(codecName, QVariant(codecName));
    }

    //containers
    ui->containerBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &containerName: recorder->supportedContainers()) {
        ui->containerBox->addItem(containerName, QVariant(containerName));
    }

    //sample rate
    ui->sampleRateBox->addItem(tr("Default"), QVariant(0));
    for (int sampleRate: recorder->supportedAudioSampleRates()) {
        ui->sampleRateBox->addItem(QString::number(sampleRate), QVariant(
                sampleRate));
    }

    //channels
    ui->channelsBox->addItem(tr("Default"), QVariant(-1));
    ui->channelsBox->addItem(QStringLiteral("1"), QVariant(1));
    ui->channelsBox->addItem(QStringLiteral("2"), QVariant(2));
    ui->channelsBox->addItem(QStringLiteral("4"), QVariant(4));

    //quality
    ui->qualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));
    ui->qualitySlider->setValue(int(QMultimedia::NormalQuality));

}


audioqualityoption::~audioqualityoption()
{
    delete ui;
}


void audioqualityoption::on_pushButton_2_clicked()
{

}

/* cancel button pressed */
void audioqualityoption::on_cancelbutton_clicked()
{
    this->close();
}

void MainWindow::on_actionQuality_setting_triggered()
{
    audioqualityoption insert(nullptr, this);
    insert.exec();
}

void audioqualityoption::on_pushButton_clicked()
{
    savequalita(this);

    this->close();
}

#endif
