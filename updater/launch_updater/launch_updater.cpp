#include "launch_updater.h"
#include "stdlib.h"
#include "src/utils/dialog_critic/dialog_critic.h"
#include "src/utils/mostra_explorer/mostra_explorer.h"

void launch_updater::launch()
{
#if defined(WIN64) || defined(WIN32)
    int res;
    res = system("C:\\'Program Files (x86)'\\writernote\\updater.exe");

    if(res){
        user_message("It was not possible to open the updater, if you want to update writernote, open the program called updater.exe");
        mostra_explorer("C:\\Program Files (x86)\\writernote\\");
    }

#endif

}
