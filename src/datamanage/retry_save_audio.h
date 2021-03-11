#ifndef RETRY_SAVE_AUDIO_H
#define RETRY_SAVE_AUDIO_H

#include <QObject>

class currenttitle_class;

class retry_save_audio: public QObject
{
    Q_OBJECT
public:
    retry_save_audio(currenttitle_class *data, bool *exist);
    ~retry_save_audio();

private:
    bool *m_exist;
    currenttitle_class *m_current;

    bool exist();

public slots:
    void retry();
    void close();

signals:
    void resultRetry(bool result);

};


#endif // RETRY_SAVE_AUDIO_H
