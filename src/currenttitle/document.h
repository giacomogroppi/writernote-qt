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

class Document :
        public DataStruct,
#ifdef PDFSUPPORT
        public frompdf,
#endif // PDFSUPPORT
        public ImageContainerDrawable
{
public:
    Document () noexcept;
    Document (const Document &other) noexcept;
    Document (Document &&other) noexcept;
    virtual ~Document();

    [[nodiscard]]
    size_t createSingleControll() const;

    [[nodiscard]]
    auto isEmpty() const -> bool;

    template <class Readable>
            requires (std::is_base_of_v<ReadableAbstract, Readable>())
    static auto read (const VersionFileController &versionController, Readable &readable, Document &doc) -> int;

    template <class Writable>
            requires (std::is_base_of_v<WritableAbstract, Writable>())
    static auto write (Writable &writable, const Document &doc) -> int;

    enum AudioRecordStatus{
        not_record,
        record_file,
        record_zip
    };

    void setRecordStatus (AudioRecordStatus status);
    AudioRecordStatus recordStatus() const;
    bool isRecorded() const;
    const WByteArray &getAudioData() const;
    const WString &getAudioPath() const;

    void reset();
    void cleanAudio();

    void controllForRepositioning();
    void increaseZoom(double delta, const WSizeF &size);
    void adjustAll(unsigned w, unsigned h);
    void adjustHeight(cdouble height);
    void scala_all(const PointF& delta, double heightView) override;
    void repositioning();

    Document &operator=(const Document &other) noexcept;
    Document &operator=(Document &&other) noexcept;

    void setAudioPath(const WString &path) noexcept;

private:
    WString _audioPositionPath;
    AudioRecordStatus _audioRecordStatus = AudioRecordStatus::not_record;
    WByteArray _audioRawData;
};

force_inline bool Document::isEmpty() const
{
    const auto res =
            isEmptyTouch() and
            #ifdef PDFSUPPORT
            frompdf::length_pdf() == 0 and
#endif // PDFSUPPORT
            ImageContainerDrawable::lengthImage() == 0;
    return res;
}

template<class Writable>
requires (std::is_base_of_v<WritableAbstract, Writable>())
auto Document::write(Writable &writable, const Document &doc) -> int
{
    if (DataStruct::write (writable, doc) < 0)
        return -1;
    if (ImageContainerDrawable::write (writable, doc) < 0)
        return -1;
    if (PdfContainerDrawable::write (writable, doc) < 0)
        return -1;
    return 0;
}
