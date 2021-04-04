#ifndef CURRENT_TITLE_CLASS_H
#define CURRENT_TITLE_CLASS_H

#include <QStringList>
#include <QList>

#include "../touch/datastruct/datastruct.h"

#include "../images/image_struct.h"

#define CURRENT_VERSION_CURRENT_TITLE 3
#define MIN_VERSION_CURRENT_TITLE 2

#include <QByteArray>

namespace audio_record {
    enum n_audio_record{
        not_record,
        record_file,
        record_zip
    };
};

class currenttitle_class{
private:
    size_t createSingleControll();
public:
    QString nome_copybook;

    int versione = CURRENT_VERSION_CURRENT_TITLE;
    bool se_tradotto = false;
    QString audio_position_path = "";
    QString testi = "";
    QStringList testinohtml = {};
    QList<int> posizione_iniz = {};

    bool isOkVersion(){
        if(this->versione < MIN_VERSION_CURRENT_TITLE)
            return false;
        return true;
    }

    bool m_touch = false;
    datastruct *datatouch;
    void reset();

    QList<struct immagine_S> immagini;

    audio_record::n_audio_record se_registato = audio_record::not_record;
    QByteArray audio_data;

    currenttitle_class();

    static void copy(currenttitle_class *src, currenttitle_class *dest);

};

#endif
