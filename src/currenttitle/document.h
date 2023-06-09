#pragma once

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

    WString audio_position_path = "";

    void reset();
    void cleanAudio();

    enum n_audio_record{
        not_record,
        record_file,
        record_zip
    };

    n_audio_record se_registato = n_audio_record::not_record;
    WByteArray audio_data;

    Document () noexcept;
    Document (const Document &other) noexcept;
    Document (Document &&other) noexcept;
    virtual ~Document();

    void controllForRepositioning();
    void increaseZoom(double delta, const WSizeF &size);
    void adjustAll(unsigned w, unsigned h);
    void adjustHeight(cdouble height);
    void scala_all(const PointF& delta, double heightView) override;
    void repositioning();

    Document &operator=(const Document &other) noexcept;
    Document &operator=(Document &&other) noexcept;
};

force_inline bool Document::isEmpty() const
{
    const auto res =
            isEmptyTouch() and
#ifdef PDFSUPPORT
            frompdf::length_pdf() == 0 and
#endif // PDFSUPPORT
            fromimage::length_img() == 0;
    return res;
}
