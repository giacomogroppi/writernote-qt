#include "get_path.h"
#include "utils/slash/slash.h"
#include "utils/get_path_application.h"
#include "core/WDir.h"
#include "file/Directory.h"

#if defined(unix) || defined(MACOS) || defined(IOS_WRITERNOTE)
#define POS_HOME "HOME"
#elif WIN32
#define POS_HOME "USERPROFILE"
#endif


/*
 * the function does not check if the
 * folder to write the audio exists
*/
WString get_path_no_controll()
{
    return get_path_application::exe();
}
