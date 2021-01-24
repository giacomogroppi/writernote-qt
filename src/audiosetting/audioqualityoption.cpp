#include "audioqualityoption.h"
#include "ui_audioqualityoption.h"
#include "../mainwindow.h"

#include "savequalita.h"
#include "../dialog_critic.h"

audioqualityoption::audioqualityoption(QWidget *parent, MainWindow *padre) :
    QDialog(parent),
    ui(new Ui::audioqualityoption)
{
    ui->setupUi(this);
    this->padre = padre;



    //audio devices
    ui->audioDeviceBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &device: this->padre->m_audioRecorder->audioInputs()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }

    //audio codecs
    ui->audioCodecBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &codecName: this->padre->m_audioRecorder->supportedAudioCodecs()) {
        ui->audioCodecBox->addItem(codecName, QVariant(codecName));
    }

    //containers
    ui->containerBox->addItem(tr("Default"), QVariant(QString()));
    for (auto &containerName: this->padre->m_audioRecorder->supportedContainers()) {
        ui->containerBox->addItem(containerName, QVariant(containerName));
    }

    //sample rate
    ui->sampleRateBox->addItem(tr("Default"), QVariant(0));
    for (int sampleRate: this->padre->m_audioRecorder->supportedAudioSampleRates()) {
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
    audioqualityoption *insert = new audioqualityoption(nullptr, this);
    insert->exec();

    delete insert;
}

void audioqualityoption::on_pushButton_clicked()
{
    if(savequalita(this))
        dialog_critic("We had a error saving the config");

    this->close();
}
