#include "audiorecord.h"
#include "core/core.h"

#ifdef USE_QT
# include <QMediaRecorder>
#endif // USE_QT

#include "utils/setting_define.h"
#include "testing/memtest.h"

AudioRecord::AudioRecord(WObject *parent,
                         Fn<void(const WString &)> error,
                         Fn<void(int)> durationChange)
    : WObject(parent)
    , _error(error)
    , _durationChange(durationChange)
{
#ifdef USE_QT
    this->recorder = new QMediaRecorder(nullptr);
    m_captureSession.setRecorder(recorder);
    m_captureSession.setAudioInput(new QAudioInput(nullptr));

    QObject::connect(recorder, &QMediaRecorder::durationChanged, [this] (unsigned long d) {
        this->updateProgress(d);
    });
#endif
}

AudioRecord::~AudioRecord()
{
#ifdef USE_QT
    delete this->recorder;
#endif // USE_QT
}

#ifdef USE_QT
void AudioRecord::updateProgress(unsigned long duration)
{
    this->_durationChange(duration / 1000);
}

void AudioRecord::displayErrorMessage()
{
    this->_error(
                recorder->errorString()
    );
}
#endif // USE_QT
