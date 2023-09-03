#pragma once


#include "utils/WCommonScript.h"

#if defined(WIN64) || defined(WIN32)
# define is_windows 1
#else
# define is_windows 0
#endif

#if defined(linux)
# define is_linux 1
#else
# define is_linux 0
#endif

#if defined(ANDROID_WRITERNOTE)
# define is_android 1
#else
# define is_android 0
#endif

#ifdef __APPLE__
#  include <TargetConditionals.h>
#  if TARGET_OS_MACCATALYST
     // Mac's Catalyst (ports iOS API into Mac, like UIKit).
#    define is_mac 1
#    define is_ios 0
#  elif TARGET_OS_IPHONE
     // iOS, tvOS, or watchOS device
#    define is_mac 0
#    define is_ios 1
#  elif TARGET_OS_MAC
     // Other kinds of Apple platforms
#    define is_mac 1
#    define is_ios 0
#  else
#    error "Unknown Apple platform"
#    endif
#else
#  define is_mac 0
#  define is_ios 0
#endif // __APPLE__

force_inline bool is_mobile_static()
{
    return is_android || is_ios;
}

static_assert(is_mac == 0 or is_mac == 1);
static_assert(is_ios == 0 or is_ios == 1);
//static_assert(is_mac + is_ios == 1);
