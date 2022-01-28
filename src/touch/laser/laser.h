#ifndef LASER_H
#define LASER_H

#include "utils/common_script.h"
#include <QTimer>
#include <QObject>
#include "testing/memtest.h"

class laser : public QObject
{
    Q_OBJECT
private:
    static constexpr int _size = 1;
    QTimer *_timer;
public:
    laser(QObject *parent);
    ~laser();

    void startMove();
    void endMode();

private slots:
    void endTimer();

};

force_inline void laser::endTimer()
{

}

force_inline void laser::startMove()
{

}

force_inline void laser::endMode()
{

}

#endif // LASER_H
