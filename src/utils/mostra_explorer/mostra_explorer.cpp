#include "mostra_explorer.h"
#include "string.h"
#include "windows/mostra_finestra_i.h"
#include "utils/slash/slash.h"
#include "../path/pathfile.h"
#include "utils/WCommonScript.h"
#include "utils/platform.h"

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const WString &comando);
#endif // WIN

#if defined(unix) and !defined(MACOS)
#define APPLICATION_NAME (WString)"xdg-open "
#elif defined(WIN32) || defined(WIN64) || defined(__OS2__)
#define APPLICATION_NAME (WString)"explorer.exe "
#elif defined(MACOS)
#define APPLICATION_NAME (WString)"open "
#elif defined(MACOS)
#define APPLICATION_NAME (WString)"open "
#endif

void mostra_explorer(const WString &posizione)
{
#if defined(WIN32) || defined(WIN64)
    WString tmp = pathFile::remove_file(posizione);
    tmp = APPLICATION_NAME + tmp;
    if(mostra(tmp)){
        dialog_critic("We had a problem opening " + APPLICATION_NAME);
    }

#elif (defined(unix) || defined(MACOS)) && !defined(SNAP)
    WString tmp = pathFile::remove_file(posizione);
    tmp = pathFile::remove_file(posizione);
    mostra_finestra_i(tmp);
#elif defined(SNAP)
    const WString path = pathFile::remove_file(posizione);
    const WString tmp = WString("%1 %2").arg(APPLICATION_NAME).arg(path);
    mostra(tmp);
#endif //SNAP
}

#if defined(WIN32) || defined(WIN64) || defined(__OS2__) || defined(SNAP)
static int mostra(const WString &comando){
    const auto res = system(comando.toUtf8().constData());

    if(is_windows){
        return res != 1;
    }

    return res;
}
#endif


