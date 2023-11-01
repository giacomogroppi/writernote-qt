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
        public PdfContainerDrawable,
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

    static auto load (const VersionFileController &versionController, 
                        ReadableAbstract &readable) -> WPair<Error, Document>;

    template <class Writable>
            requires (std::is_base_of<WritableAbstract, Writable>::value)
    static auto write (Writable &writable, const Document &doc) -> Error;

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

    auto operator=(const Document &other) noexcept -> Document &;
    auto operator=(Document &&other) noexcept -> Document &;

    void setAudioPath(const WString &path) noexcept;

    /**
     * \brief This method need to be called in the main function before do anything on a document and
     * all the classed he use.
     * This method require that a instance of Scheduler is present
     * */
    static void init();

private:
    WString _audioPositionPath;
    AudioRecordStatus _audioRecordStatus = AudioRecordStatus::not_record;
    WByteArray _audioRawData;
};

force_inline bool Document::isEmpty() const
{
    const auto res =
            isEmptyTouch() and
            PdfContainerDrawable::length_pdf() == 0 and
            ImageContainerDrawable::lengthImage() == 0;
    return res;
}

template<class Writable> requires (std::is_base_of<WritableAbstract, Writable>::value)
auto Document::write(Writable &writable, const Document &doc) -> Error
{
    if (auto err = DataStruct::write (writable, doc))
        return err;
    if (auto err = ImageContainerDrawable::write (writable, doc))
        return err;
    if (auto err = PdfContainerDrawable::write (writable, doc))
        return err;
    return Error::makeOk();
}

inline auto Document::getAudioPath() const -> const WString &
{
    return this->_audioPositionPath;
}

inline auto Document::getAudioData() const -> const WByteArray &
{
    return this->_audioRawData;
}
