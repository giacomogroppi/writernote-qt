#include "removenotwriternote.h"
#include "core/WListFast.h"
#include "core/WString.h"
#include "utils/WCommonScript.h"

void removeNotWriternote::exe(WListFast<QString> &ref)
{
    int i;
    const auto ext = qstr("." APP_EXT);

    for(i = 0; i < ref.length(); i++){
        if(ref.at(i).indexOf(ext) == -1){
            ref.removeAt(i);
            i--;
        }
    }
}
