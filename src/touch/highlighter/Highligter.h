#pragma once

#include <QObject>
#include "HighligterMethod.h"

class Highligter: public QObject, public HighligterMethod {
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
    explicit Highligter(QObject *parent,
                        std::function<int()> getTime,
                        std::function<void(const QPointF &)> objectMove,
                        QColor &color,
                        QPen &pen);

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
