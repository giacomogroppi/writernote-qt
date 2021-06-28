#ifndef CURRENT_TITLE_CLASS_H
#define CURRENT_TITLE_CLASS_H

#include <QStringList>
#include <QList>

#include "../touch/datastruct/datastruct.h"
#include "../images/image_struct.h"

#define CURRENT_VERSION_CURRENT_TITLE 4
#define MIN_VERSION_CURRENT_TITLE 2

#include <QByteArray>

#define DEF_METHOD true

class currenttitle_class{
public:
    size_t createSingleControll() const;

    bool isEmpty() const;

    QString nome_copybook;

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

    QList<struct immagine_S> immagini;

    enum n_audio_record{
        not_record,
        record_file,
        record_zip
    };

    n_audio_record se_registato = n_audio_record::not_record;
    QByteArray audio_data;

    currenttitle_class();
    ~currenttitle_class();

    static void copy(const currenttitle_class *src, currenttitle_class *dest);

};

#endif
