#pragma once


#include "utils/WCommonScript.h"
#include <chrono>

namespace utils{

force_inline unsigned long int get_time_since_epoch(){
    using namespace std::chrono;
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}

}

