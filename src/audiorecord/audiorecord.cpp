#include "audiorecord.h"
#include "core/core.h"

#ifdef USE_QT
#include <QMediaRecorder>
#endif // USE_QT

#include "utils/setting_define.h"
#include "testing/memtest.h"

AudioRecord::AudioRecord(WObject *parent,
                         std::function<void(const WString &)> error,
                         std::function<void(int)> durationChange)
    : WObject(parent)
    , _error(error)
    , _durationChange(durationChange)
{
#ifdef USE_QT
    this->recorder = new QMediaRecorder(this);
    m_captureSession.setRecorder(recorder);
    m_captureSession.setAudioInput(new QAudioInput(this));

    QObject::connect(recorder, &QMediaRecorder::durationChanged, this, &AudioRecord::updateProgress);
#endif
}
#ifdef USE_QT
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
#endif // USE_QT