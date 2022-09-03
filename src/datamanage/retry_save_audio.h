#pragma once

#include <QObject>

class Document;

class retry_save_audio: public QObject
{
    Q_OBJECT
public:
    retry_save_audio(Document *data, bool *exist);
    ~retry_save_audio();

private:
    bool *m_exist;
    Document *m_current;

    bool exist();

public slots:
    void retry();
    void close();

signals:
    void resultRetry(bool result);

};

