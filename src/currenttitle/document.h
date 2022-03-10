#ifndef CURRENT_TITLE_CLASS_H
#define CURRENT_TITLE_CLASS_H

#include <QStringList>
#include <QList>
#include <QByteArray>
#include "touch/datastruct/datastruct.h"

class fromimage;
class frompdf;
#define NAME_FILE "data.xml"
#define NAME_BIN "data_bin.xml"
#define NAME_AUDIO "data_audio.mp4"

#define CURRENT_VERSION_STROKE 2
#define CURRENT_VERSION_CURRENT_TITLE 8
#define MIN_VERSION_CURRENT_TITLE 2

class Document{
private:
    void init();
public:
    size_t createSingleControll() const;

    bool isEmpty() const;

    frompdf *m_pdf = NULL;
    fromimage *m_img = NULL;

    int versione = CURRENT_VERSION_CURRENT_TITLE;
    QString audio_position_path = "";

    bool isOkVersion() const{
        return this->versione >= MIN_VERSION_CURRENT_TITLE;
    }

    datastruct *datatouch = NULL;
    void reset();
    void cleanAudio();
    uint count_pdf = 0;
    uint count_img = 0;

    enum n_audio_record{
        not_record,
        record_file,
        record_zip
    };

    n_audio_record se_registato = n_audio_record::not_record;
    QByteArray audio_data;

    Document();
    Document(const Document &);

    ~Document();

    static void copy(const Document &src, Document &dest);
};

force_inline bool Document::isEmpty() const
{
    return datatouch->isempty();
}

#endif
