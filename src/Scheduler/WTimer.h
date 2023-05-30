//
// Created by Giacomo Groppi on 10/05/23.
//

#ifndef WRITERNOTE_WTIMER_H
#define WRITERNOTE_WTIMER_H

#include <thread>
#include <chrono>
#include <iostream>
#include "WObject.h"
#include "core/AtomicSafe.h"
#include "core/WRecursiveLock.h"

class WTimer: public WObject {
private:
    mutable WRecursiveLock _lock;
    int _currentId = 0;
    bool _isActive;
    int _millisecond;

    std::function<void()> _function;
public:
    WTimer(WObject *parent, std::function<void()> function, int millisecond);
    ~WTimer() = default;

    bool isActive() const;
    void stop();
    void start(int millisecond = -1);
    void setSingleShot(bool singleShot);

    bool isSingleShot() const noexcept;
};


#endif //WRITERNOTE_WTIMER_H
