#pragma once

#ifdef USE_QT
# include "WPainterQt.h"
#else
// TODO: ANDROID include
# include "WPainterIos.h"
#endif // USE_QT
