#include "launch_updater.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/mostra_explorer/mostra_explorer.h"

void launch_updater::launch()
{
#if defined(WIN64) || defined(WIN32)
    int res;
    const char *command = "\"C:\\Program Files (x86)\\writernote\\updater.exe\"";
    res = system(command);

    //res = execl("C::\\'Program Files (x86)'\\writernote\\updater.exe", nullptr);

    if(res){
        user_message("It was not possible to open the updater, if you want to update writernote, open the program called updater.exe");
        mostra_explorer("C:\\Program Files (x86)\\writernote\\");
    }

#endif

}
