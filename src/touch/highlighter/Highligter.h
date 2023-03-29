#pragma once

#include <QObject>
#include "HighligterMethod.h"

class Highligter: public QObject, public HighligterMethod {
private:
    enum HighligterType {
        HighligterTypePressure,
        HighligterTypeConstSize
    };

    HighligterType _type;
    pressure_t _size;
    int _alfa;

    uchar getAlfa() const override;
public:
    explicit Highligter(QObject *parent,
                        std::function<int()> getTime,
                        std::function<void()> objectMove,
                        QColor &color,
                        QPen &pen);

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


