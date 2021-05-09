#include "launch_updater.h"
#include "stdlib.h"

void launch_updater::launch()
{
#if defined(WIN64) || defined(WIN32)
    int res;
    res = system("C:\\Program Files (x86)\\writernote\\updater.exe");

#endif

}
