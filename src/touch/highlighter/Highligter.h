#pragma once

#include "Scheduler/WObject.h"
#include "HighligterMethod.h"

class Highligter: public WObject, public HighligterMethod {
    Q_OBJECT
private:
    enum HighligterType {
        HighligterTypePressure,
        HighligterTypeConstSize
    };

    HighligterType _type;
    pressure_t _size;
    int _alfa;

    [[nodiscard]] uchar getAlfa() const override;
public:
    explicit Highligter(WObject *parent,
                        std::function<int()> getTime,
                        std::function<void(const PointF &)> objectMove,
                        colore_s &color,
                        WPen &pen);

    virtual ~Highligter() = default;

    int getType() const final;
    static constexpr int type();

public slots:
    void setHighligterPressureConst();
    void setHighligterSize();

    void setSize(double size);
    void setAlfa(int alfa);

public:
    pressure_t getSize(double pressure);

signals:
    void onSizeChanged();
    void onTypeChanged();
    void onAlfaChanged();
};


inline constexpr int Highligter::type()
{
    return 2;
}
