#ifndef CURRENT_TITLE_CLASS_H
#define CURRENT_TITLE_CLASS_H

#include <QStringList>
#include <QList>

#include "../touch/datastruct/datastruct.h"

class fromimage;
class frompdf;
#define NAME_FILE "data.xml"
#define NAME_BIN "data_bin.xml"
#define NAME_AUDIO "data_audio.mp4"

#define CURRENT_VERSION_CURRENT_TITLE 6
#define MIN_VERSION_CURRENT_TITLE 2

#include <QByteArray>

#define DEF_METHOD true

class Document{
public:
    size_t createSingleControll() const;

    bool isEmpty() const;

    frompdf *m_pdf;
    fromimage *m_img;

    int versione = CURRENT_VERSION_CURRENT_TITLE;
    bool se_tradotto = false;
    QString audio_position_path = "";
    QString testi = "";
    QStringList testinohtml = {};
    QList<int> posizione_iniz = {};

    bool isOkVersion() const{
        return this->versione >= MIN_VERSION_CURRENT_TITLE;
    }

    bool m_touch = DEF_METHOD;
    datastruct *datatouch;
    void reset();

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
    ~Document();

    static void copy(const Document *src, Document *dest);

};

#endif
