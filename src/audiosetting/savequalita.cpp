#include "savequalita.h"
#include "audioqualityoption.h"
#include "ui_audioqualityoption.h"
#include "utils/setting_define.h"
#include <QSettings>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void savequalita(audioqualityoption *padredialog){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO);

    setting.setValue(KEY_AUDIO_DEVICE_BOX,
                     boxValue(padredialog->ui->audioDeviceBox).toString().toUtf8().constData());

    setting.setValue(KEY_SAMPLE_RATE_BOX,
                     boxValue(padredialog->ui->sampleRateBox).toInt());

    setting.setValue(KEY_AUDIO_CODEC_BOX,
                     boxValue(padredialog->ui->audioCodecBox).toString().toUtf8().constData());

    setting.setValue(KEY_CHANNELS_BOX,
                     boxValue(padredialog->ui->channelsBox).toInt());

    setting.setValue(KEY_QUALITY_SLIDER,
                     padredialog->ui->qualitySlider->value());


    setting.setValue(KEY_CONTAINER_BOX,
                     boxValue(padredialog->ui->containerBox).toString().toUtf8().constData());


    setting.endGroup();
}

#endif
