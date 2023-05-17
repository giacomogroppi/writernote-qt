//
// Created by Giacomo Groppi on 10/05/23.
//

#ifndef WRITERNOTE_WTIMER_H
#define WRITERNOTE_WTIMER_H

#include <thread>
#include <chrono>
#include <iostream>
#include "WObject.h"

class WTimer: public WObject {
private:
    int _millisecond;
    std::function<void()> _function;
public:
    WTimer(WObject *parent, std::function<void()> function, int millisecond);
    ~WTimer() = default;

    bool isActive() const;
    void stop();
    void start(int millisecond = -1);
    void setSingleShot(bool singleShot);
};


#endif //WRITERNOTE_WTIMER_H
