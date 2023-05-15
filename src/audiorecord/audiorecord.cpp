#include "audiorecord.h"
#include <QFileInfo>
#include <QSettings>
#include "core/core.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAudioEncoderSettings>
#else
#include <QMediaRecorder>
#endif

#include "utils/setting_define.h"
#include "testing/memtest.h"

AudioRecord::AudioRecord(WObject *parent,
                         std::function<void(const QString &)> error,
                         std::function<void(int)> durationChange)
    : WObject(parent)
    , _error(error)
    , _durationChange(durationChange)
{
    this->recorder = new QMediaRecorder(this);
    m_captureSession.setRecorder(recorder);
    m_captureSession.setAudioInput(new QAudioInput(this));

    WObject::connect(recorder, &QMediaRecorder::durationChanged, this, &AudioRecord::updateProgress);
}

void AudioRecord::updateProgress(qint64 duration)
{
    if (this->errors() != QMediaRecorder::NoError)
        return;

    this->_durationChange(duration / 1000);
}

void AudioRecord::displayErrorMessage()
{
    this->_error(
                recorder->errorString()
    );
}
