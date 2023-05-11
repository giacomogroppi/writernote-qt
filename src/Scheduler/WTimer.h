//
// Created by Giacomo Groppi on 10/05/23.
//

#ifndef WRITERNOTE_WTIMER_H
#define WRITERNOTE_WTIMER_H

#include <thread>
#include <chrono>
#include <iostream>

template <typename Function>
class WTimer {
private:
    int millisecond;
public:
    WTimer(int millisecond);
    ~WTimer() = default;

    void start();

};

template<typename Function>
inline void WTimer<Function>::start()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
    Function();
}


#endif //WRITERNOTE_WTIMER_H
