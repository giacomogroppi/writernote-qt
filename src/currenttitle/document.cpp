#include "document.h"
#include "core/String/WString.h"

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

Document::Document() noexcept
    : DataStruct()
    , PdfContainerDrawable()
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
    int counterPage, counterStroke;
    const auto lenPage = lengthPage();
    int lenStroke;

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

auto Document::increaseZoom(double delta, const WSizeF &size) -> void
{
    PointF res;

    DataStruct::increaseZoom(delta, size, res);

    Document::scala_all(res, size.getHeight());
}

auto Document::controllForRepositioning() -> void
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

auto Document::operator=(const Document &other) noexcept -> Document &
{
    if (this == &other)
        return *this;
    DataStruct::operator=(other);
    ImageContainerDrawable::operator=(other);

    PdfContainerDrawable::operator=(other);

    return *this;
}

auto Document::operator=(Document &&other) noexcept -> Document &
{
    if (this == &other)
        return *this;

    DataStruct::operator=(std::move(static_cast<DataStruct&>(other)));
    ImageContainerDrawable::operator=(std::move(static_cast<ImageContainerDrawable &>(other)));

    PdfContainerDrawable::operator=(std::move(static_cast<PdfContainerDrawable&>(other)));

    _audioRecordStatus = other._audioRecordStatus;
    _audioRawData = std::move(other._audioRawData);
    _audioPositionPath = std::move(other._audioPositionPath);

    return *this;
}

void Document::setAudioPath(const WString &path) noexcept
{
    this->_audioPositionPath = path;
}

// TODO: Remove this rep method
static void test()
{
    Document doc;
    FileWriter tmp("prova");
    const bool t = std::is_base_of_v<WritableAbstract, FileWriter>;
    Document::write(tmp, doc);

    WFile c(WByteArray("ciao"));

    doc = std::move(Document::load(VersionFileController(), c).second);
}

void Document::setRecordStatus(Document::AudioRecordStatus status)
{
    this->_audioRecordStatus = status;
}

auto Document::getAudioPath() const -> const WString &
{
    // TODO: move this function in .h file
    return this->_audioPositionPath;
}

auto Document::isRecorded() const -> bool
{
    return this->_audioRecordStatus != Document::AudioRecordStatus::not_record;
}

auto Document::getAudioData() const -> const WByteArray &
{
    // TODO: move this function in .h file
    return this->_audioRawData;
}

auto Document::recordStatus() const -> Document::AudioRecordStatus
{
    return this->_audioRecordStatus;
}

auto Document::load(const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, Document>
{
    WPair<int, Document> result (-1, Document());

    {
        auto [res, data] = DataStruct::load (versionController, readable);
        if (res < 0)
            return result;
        static_cast<DataStruct&>(result.second) = std::move (data);
    }

    {
        auto [res, data] = ImageContainerDrawable::load (versionController, readable);
        if (res < 0)
            return result;
        static_cast<ImageContainerDrawable&>(result.second) = std::move (data);
    }

    {
        auto [res, data] = PdfContainerDrawable::load (versionController, readable);
        if (res < 0)
            return result;
        static_cast<PdfContainerDrawable&>(result.second) = std::move (data);
    }

    result.first = 0;
    return result;
}
