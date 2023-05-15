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
    using namespace WCommonScript;
    auto &l = this->_data->_point;

    for_each(stroke._point, [&l](const Point &point) {
        l.append(point);
    });
    rep();
}

int StrokeForPage::load(WZipReaderSingle &reader, int ver_stroke)
{
    int ok;
    auto *tmp =
            dynamic_cast<StrokeNormal*>(
                    Stroke::load(reader, ver_stroke, &ok).get()
            );

    if(ok != OK){

            delete tmp;
        return ok;
    }

    this->_data = std::unique_ptr<StrokeNormal>(tmp);

    if(_data->_pressure[0] > 10)
        _data->_pressure[0] = 1.5;

    rep();
    return OK;
}

size_t StrokeForPage::getSizeInFile() const
{
    const auto res = this->_data->getSizeInFile();
    rep();
    return res;
}

int StrokeForPage::save(WZipWriterSingle &writer) const
{
    const auto res = this->_data->save(writer);
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

void StrokeForPage::setMetadata(const colore_s &colore)
{
    _data->setMetadata(-1, colore);
    rep();
}

void StrokeForPage::draw(WPainter &painter, double delta,
                         const Page &page, const WSize &target,
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

    const auto source = Rect(_pix.rect().castTo<int>())
                        / PROP_RESOLUTION;

    const auto sourceDraw = Rect{source.intersected(visibleArea.castTo<int>())} * PROP_RESOLUTION;

    if (!source.isNull())
        painter.drawPixmap(
                targetRect,
                this->_pix,
                sourceDraw
            );

    //painter.drawLine(0, 0, 2000, 5000);

    rep();
}

void StrokeForPage::draw() const
{
    _pix = WPixmap(1, true);
    _pix.fill(color_transparent);
    WPainter painter;
    painter.begin(&_pix);
    WPen pen;

    const auto lenPoint = this->_data->_point.size();

    if(!lenPoint)
        return;

    auto pressure = _data->getPressure();

    pressure = TabletUtils::pressureToWidth(pressure / 2.0);

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
