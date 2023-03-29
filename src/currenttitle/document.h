#pragma once

#include <QList>
#include <QByteArray>
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

#define NAME_FILE "data.xml"
#define NAME_BIN "data_bin.xml"
#define NAME_AUDIO "data_audio.mp4"

#define CURRENT_VERSION_STROKE 2
#define CURRENT_VERSION_CURRENT_TITLE 9
#define MIN_VERSION_CURRENT_TITLE 2

class Document : public DataStruct,
#ifdef PDFSUPPORT
                 public frompdf,
#endif // PDFSUPPORT
                 public fromimage
        {
private:
public:
    [[nodiscard]] size_t createSingleControll() const;

    [[nodiscard]] bool isEmpty() const;

    QString audio_position_path = "";

    void reset();
    void cleanAudio();

    enum n_audio_record{
        not_record,
        record_file,
        record_zip
    };

    n_audio_record se_registato = n_audio_record::not_record;
    QByteArray audio_data;

    Document();
    Document(const Document &);

    virtual ~Document();

    void controllForRepositioning();
    void increaseZoom(double delta, const QSize &size);
    void adjustAll(unsigned w, unsigned h);
    void adjustHeight(cdouble height);
    void scala_all(const QPointF& delta, int heightView) override;
    void repositioning();

    static void copy(const Document &src, Document &dest);
};

force_inline bool Document::isEmpty() const
{
    const auto res =
            isempty_touch() and
#ifdef PDFSUPPORT
            frompdf::length_pdf() == 0 and
#endif // PDFSUPPORT
            fromimage::length_img() == 0;
    return res;
}
