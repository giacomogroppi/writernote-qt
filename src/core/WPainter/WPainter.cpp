#include "WPainter.h"

#if defined(USE_QT)

WPainter::~WPainter()
{
    if (_allocated) {
        delete this->_painter;
    }
}

void WPainter::setAntialiasing()
{
    _painter->setRenderHints(QPainter::Antialiasing);
    _painter->setRenderHint(QPainter::SmoothPixmapTransform);
    _painter->setRenderHint(QPainter::TextAntialiasing);
}

void WPainter::setBrush(const QBrush &brush)
{
    this->_painter->setBrush(brush);
}

void WPainter::move(const PointF &point) noexcept
{
    _path.moveTo(QPointF(point.x(), point.y()));
}

class RoundedPolygon : public QPolygonF
{
public:
    RoundedPolygon()
    {    SetRadius(10); }
    void SetRadius(unsigned int iRadius)
    {    m_iRadius = iRadius; }
    const QPainterPath& GetPath();

private:
    QPointF GetLineStart(int i) const;
    QPointF GetLineEnd(int i) const;
    float GetDistance(QPointF pt1, QPointF pt2) const;
private:
    QPainterPath m_path;
    unsigned int m_iRadius;
};

float RoundedPolygon::GetDistance(QPointF pt1, QPointF pt2) const
{
    float fD = (pt1.x() - pt2.x())*(pt1.x() - pt2.x()) +
               (pt1.y() - pt2.y()) * (pt1.y() - pt2.y());
    return sqrtf(fD);
}

QPointF RoundedPolygon::GetLineStart(int i) const
{
    QPointF pt;
    QPointF pt1 = at(i);
    QPointF pt2 = at((i+1) % count());
    float fRat = m_iRadius / GetDistance(pt1, pt2);
    if (fRat > 0.5f)
        fRat = 0.5f;

    pt.setX((1.0f-fRat)*pt1.x() + fRat*pt2.x());
    pt.setY((1.0f-fRat)*pt1.y() + fRat*pt2.y());
    return pt;
}

QPointF RoundedPolygon::GetLineEnd(int i) const
{
    QPointF pt;
    QPointF pt1 = at(i);
    QPointF pt2 = at((i+1) % count());
    float fRat = m_iRadius / GetDistance(pt1, pt2);
    if (fRat > 0.5f)
        fRat = 0.5f;
    pt.setX(fRat*pt1.x() + (1.0f - fRat)*pt2.x());
    pt.setY(fRat*pt1.y() + (1.0f - fRat)*pt2.y());
    return pt;
}

const QPainterPath& RoundedPolygon::GetPath()
{
    m_path = QPainterPath();

    if (count() < 3) {
        qWarning() << "Polygon should have at least 3 points!";
        return m_path;
    }

    QPointF pt1;
    QPointF pt2;
    for (int i = 0; i < count(); i++) {
        pt1 = GetLineStart(i);

        if (i == 0)
            m_path.moveTo(pt1);
        else
            m_path.quadTo(at(i), pt1);

        pt2 = GetLineEnd(i);
        m_path.lineTo(pt2);
    }

    // close the last corner
    pt1 = GetLineStart(0);
    m_path.quadTo(at(0), pt1);

    return m_path;
}

void WPainter::addCurve(const PointF &to, const PointF &control) noexcept
{
    using namespace WCommonScript;

    const PointF start = {_path.currentPosition().x(), _path.currentPosition().y()};

    RoundedPolygon poly;

    poly << _path.currentPosition() << to << control;

    _painter->drawPolygon(poly);
}

void WPainter::closePath () noexcept
{
    //_painter->drawPath(_path);
    _path = QPainterPath();
}

#endif // USE_QT
