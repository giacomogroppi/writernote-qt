#include "AudioRecorderQt.h"
#include "core/core.h"

#ifdef USE_QT

#include <QMediaRecorder>
#include <QAudioInput>

#include "utils/setting_define.h"

AudioRecord::AudioRecord(WObject *parent,
                         Fn<void(const WString &)> error)
        : WObject(parent)
        , _error(std::move(error))
{
    this->recorder = new QMediaRecorder(nullptr);
    m_captureSession.setRecorder(recorder);
    m_captureSession.setAudioInput(new QAudioInput(nullptr));

    QObject::connect(recorder, &QMediaRecorder::recorderStateChanged, [this] (QMediaRecorder::RecorderState status) {
        if (status == QMediaRecorder::StoppedState) {
            WByteArray result;
            if (WFile::readFile(result, this->_currentPosition) < 0) {
                return this->_error("Error loading file from disk");
            }

            AudioData audioData (std::move(result));

            W_EMIT_1(onDataChanged,  audioData);
        }
    });

    QObject::connect(recorder, &QMediaRecorder::durationChanged, [this] (unsigned long d) {
        W_ASSERT(d >= 0 and d < UINT_MAX);

        W_EMIT_1(onDurationChanged, Unsigned(static_cast<unsigned>(d / 1000u)));
    });
}

AudioRecord::~AudioRecord()
{
    delete this->recorder;
}

void AudioRecord::displayErrorMessage()
{
    this->_error(
            recorder->errorString()
    );
}

#endif // USE_QT
