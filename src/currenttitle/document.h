#pragma once

#include <QList>
#include <QByteArray>
#include "touch/dataTouch/datastruct/datastruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

#define NAME_FILE "data.xml"
#define NAME_BIN "data_bin.xml"
#define NAME_AUDIO "data_audio.mp4"

#define CURRENT_VERSION_STROKE 2
#define CURRENT_VERSION_CURRENT_TITLE 9
#define MIN_VERSION_CURRENT_TITLE 2

class Document{
private:
    void init();
public:
    [[nodiscard]] size_t createSingleControll() const;

    [[nodiscard]] bool isEmpty() const;

    frompdf *m_pdf;
    fromimage *m_img;

    int versione = CURRENT_VERSION_CURRENT_TITLE;
    QString audio_position_path = "";

    datastruct *datatouch;
    void reset();
    void cleanAudio();
    unsigned count_pdf = 0;
    unsigned count_img = 0;

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
