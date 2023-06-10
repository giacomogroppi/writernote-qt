#include "document.h"
#include "core/WString.h"

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

Document::Document() noexcept
    : DataStruct()
#ifdef PDFSUPPORT
    , frompdf()
#endif // PDFSUPPORT
    , ImageContainerDrawable()
{
}

Document::Document(const Document &src) noexcept = default;

Document::~Document() = default;

void Document::scala_all(const PointF &delta, double heightView)
{
    DataStruct::scala_all(delta, heightView);
    ImageContainerDrawable::moveImage(delta);
#ifdef PDFSUPPORT
    frompdf::translation_pdf(delta);
#endif // PDFSUPPORT
}

void Document::repositioning()
{
    DataStruct::setZoom(1.);
    DataStruct::setPageVisible(-1);

    const PointF point = this->getPointFirstPageNoZoom() * -1.;

    Document::scala_all(point, INT_MAX);
}

/*
void Document::copy(const Document &src, Document &dest)
{
    dest._audioPositionPath = src._audioPositionPath;

    static_cast<DataStruct &>(dest) = static_cast<const DataStruct &> (src);
#ifdef PDFSUPPORT
    frompdf::copy_pdf(src, dest);
#endif //PDFSUPPORT
    ImageContainerDrawable::copy_img(src, dest);
    dest._audioRecordStatus = src._audioRecordStatus;
}
*/

size_t Document::createSingleControll() const
{
    size_t ctrl = 0;
    uint counterPage, counterStroke;
    const uint lenPage = lengthPage();
    uint lenStroke;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        const Page &page = at(counterPage);
        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            ctrl += page.atStroke(counterStroke).createControll();
        }
    }

    return ctrl;
}

void Document::reset()
{
    this->_audioRecordStatus = Document::not_record;
    this->_audioPositionPath = "";

    this->_audioRawData.clear();

    reset_touch();
#ifdef PDFSUPPORT
    reset_pdf();
#endif // PDFSUPPORT
    reset_img();
}

void Document::cleanAudio()
{
    const auto lenPage = lengthPage();

    for(auto i = 0; i < lenPage; i++){
        Page &page = this->at_mod(i);
        const auto len = page.lengthStroke();

        for(auto k = 0; k < len; k++){
            page.atStrokeMod(k).clearAudio();
        }
    }

    this->_audioRecordStatus = Document::AudioRecordStatus::not_record;
}

void Document::adjustHeight(cdouble height)
{
    PointF res;
    DataStruct::adjustHeight(height, res);

    Document::scala_all(res, static_cast<int>(height));
}

void Document::adjustAll(unsigned w, unsigned h)
{
    PointF res;
    DataStruct::adjustAll(w, h, res);

    Document::scala_all(res, static_cast<int>(h));
}

void Document::increaseZoom(double delta, const WSizeF &size)
{
    PointF res;

    DataStruct::increaseZoom(delta, size, res);

    Document::scala_all(res, size.getHeight());
}

void Document::controllForRepositioning()
{
    PointF res;
    DataStruct::controllForRepositioning(res);
    Document::scala_all(res, INT_MAX);
}

Document::Document(Document &&other) noexcept
    : DataStruct(std::move (other))
    , ImageContainerDrawable(std::move (other))
#ifdef PDFSUPPORT
    , frompdf(std::move(other))
#endif // PDFSUPPORT
    , _audioRecordStatus(other._audioRecordStatus)
    , _audioRawData(std::move(other._audioRawData))
    , _audioPositionPath(std::move(other._audioPositionPath))
{

}

Document &Document::operator=(const Document &other) noexcept
{
    if (this == &other)
        return *this;
    DataStruct::operator=(other);
    ImageContainerDrawable::operator=(other);

#ifdef PDFSUPPORT
    frompdf::operator=(other);
#endif // PDFSUPPORT

    return *this;
}

Document &Document::operator=(Document &&other) noexcept
{
    if (this == &other)
        return *this;

    DataStruct::operator=(std::move(static_cast<DataStruct&>(other)));
    ImageContainerDrawable::operator=(std::move(static_cast<ImageContainerDrawable &>(other)));

#ifdef PDFSUPPORT
    frompdf::operator=(std::move(static_cast<frompdf&>(other)));
#endif // PDFSUPPORT

    _audioRecordStatus = other._audioRecordStatus;
    _audioRawData = std::move(other._audioRawData);
    _audioPositionPath = std::move(other._audioPositionPath);

    return *this;
}

void Document::setAudioPath(const WString &path) noexcept
{
    this->_audioPositionPath = path;
}

void Document::setRecordStatus(Document::AudioRecordStatus status)
{
    this->_audioRecordStatus = status;
}

const WString &Document::getAudioPath() const
{
    // TODO: move this function in .h file
    return this->_audioPositionPath;
}

bool Document::isRecorded() const
{
    return this->_audioRecordStatus != Document::AudioRecordStatus::not_record;
}

const WByteArray &Document::getAudioData() const
{
    // TODO: move this function in .h file
    return this->_audioRawData;
}

Document::AudioRecordStatus Document::recordStatus() const
{
    return this->_audioRecordStatus;
}
