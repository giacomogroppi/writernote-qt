#include "StrokeForPage.h"
#include "Stroke.h"
#include "StrokeNormal.h"
#include "utils/common_error_definition.h"
#include "touch/TabletUtils.h"
#include "core/Rect.h"

StrokeForPage::StrokeForPage()
    : _data(new StrokeNormal)
    , _needToUpdate(true)
{
    //this->_pix = WPixmap(1, true);

    rep();
}

void StrokeForPage::setPressure(pressure_t press)
{
    this->_data->force_pressure(press);
    rep();
}

void StrokeForPage::append(const StrokeNormal &stroke)
{
    using namespace WUtils;
    auto &l = this->_data->_point;

    l.append(stroke._point);

    rep();
}

size_t StrokeForPage::getSizeInFile() const
{
    const auto res = this->_data->getSizeInFile();
    rep();
    return res;
}

void StrokeForPage::scale(const PointF &delta)
{
    this->_data->scale(delta);
    rep();
}

void StrokeForPage::append(const PointF &point, pressure_t pressure)
{
    _needToUpdate = true;

    W_ASSERT(point.x() >= 0.);
    W_ASSERT(point.x() < Page::getWidth());
    W_ASSERT(point.y() >= 0.);
    W_ASSERT(point.y() < Page::getHeight());

    _data->append(point, pressure / 2.);

    rep();
}

void StrokeForPage::reset()
{
    this->_data = std::shared_ptr<StrokeNormal>(new StrokeNormal);
    rep();
}

StrokeForPage &StrokeForPage::operator=(const StrokeForPage &other)
{
    {
        std::shared_ptr<Stroke> res = other._data->clone();
        this->_data = std::dynamic_pointer_cast<StrokeNormal>(res);
    }

    rep();
    return *this;
}

void StrokeForPage::setMetadata(const WColor &colore)
{
    _data->setMetadata(-1, colore);
    rep();
}

void StrokeForPage::draw(WPainter &painter, double delta,
                         const Page &page, const WSizeF &target,
                         const RectF &visibleArea) const
{
    if (this->_needToUpdate) {
        this->draw();
        _needToUpdate = false;
    }

    const auto targetRect = RectF{
        (
                PointF(
                        0.,
                        Page::getHeight() * page.getIndex() * delta),
                        target
        )
    }.intersected(visibleArea);

    //qDebug() << page.getIndex() << "Target: " << target << " targetrect: " << targetRect;

    const auto source = RectF(_pix.rect().castTo<double>())
                        / PROP_RESOLUTION;

    const auto sourceDraw = RectF{source.intersected(visibleArea)} * PROP_RESOLUTION;

    if (!source.isNull())
        painter.drawPixmap(
                targetRect,
                this->_pix,
                sourceDraw
            );

    rep();
}

void StrokeForPage::draw() const
{
    _pix = WPixmap(1, true);
    _pix.fill({color_transparent});
    WPainterSafe painter;
    painter.begin(&_pix);
    WPen pen;

    const auto lenPoint = this->_data->_point.size();

    if(!lenPoint)
        return;

    auto pressure = _data->getPressure();

    pressure = TabletUtils::pressureToWidth(pressure / 2.);

    pen.setWidthF(pressure);
    pen.setColor(_data->getColor());

    painter.setPen(pen);

    for (auto counterPoint = 0; counterPoint < lenPoint; counterPoint += 2) {
        auto p1 = this->_data->_point.at(counterPoint);
        auto p2 = this->_data->_point.at(counterPoint + 1);

        p1 = PointF {
            p1.x(),
            p1.y()
        };
        p2 = PointF {
            p2.x(),
            p2.y()
        };

        p1 *= PROP_RESOLUTION;
        p2 *= PROP_RESOLUTION;

        painter.drawLine(p1, p2);
    }
    painter.end();
}

force_inline void StrokeForPage::rep() const
{
    W_ASSERT(this->_data.unique());
    for (const auto &p : this->_data->_point) {
        W_ASSERT(p.y() <= Page::getHeight() and p.y() >= 0.);
    }
}

StrokeForPage::StrokeForPage(StrokeForPage &&other) noexcept
    : _data(std::move(other._data))
    , _pix(std::move(other._pix))
    , _needToUpdate(other._needToUpdate)
{
}

StrokeForPage &StrokeForPage::operator=(StrokeForPage &&other) noexcept
{
    this->_data = std::move(other._data);
    this->_pix = std::move(other._pix);
    this->_needToUpdate = other._needToUpdate;
    return *this;
}

auto StrokeForPage::operator==(const StrokeForPage &other) const noexcept -> bool
{
    const auto needToUpdateEquals = this->_needToUpdate == other._needToUpdate;
    const auto dataEquals = *_data == *other._data;
    const auto pixmapEquals = this->_pix == other._pix;

    return needToUpdateEquals and dataEquals and pixmapEquals;
}

auto StrokeForPage::load(const VersionFileController &versionController,
                         ReadableAbstract &readable) -> WPair<Error, StrokeForPage>
{
    StrokeForPage result;

    if (versionController.getVersionStrokeForPage() != 0)
        return {Error::makeErrVersion(), {}};

    {
        auto [res, data] = SharedPtr<Stroke>::load(versionController, readable);
        if (res)
            return {res, {}};
        result._data = std::dynamic_pointer_cast<StrokeNormal>(data);
    }

    {
        auto [res, data] = WPixmap::load(versionController, readable);
        if (res)
            return {res, {}};
        result._pix = std::move(data);
    }

    if (auto err = readable.read(result._needToUpdate))
        return {err, {}};

    return {Error::makeOk(), std::move(result)};
}

auto StrokeForPage::write(WritableAbstract &writable, const StrokeForPage &strokeForPage) -> Error
{
    if (auto err = SharedPtr<Stroke>::write(writable, std::dynamic_pointer_cast<Stroke>(strokeForPage._data)))
        return err;

    if (auto err = WPixmap::write(writable, strokeForPage._pix))
        return err;

    if (auto err = writable.write(strokeForPage._needToUpdate))
        return err;

    return Error::makeOk();
}
