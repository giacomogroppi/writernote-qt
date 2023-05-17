#include "removenotwriternote.h"
#include "core/WListFast.h"
#include "core/WString.h"
#include "utils/WCommonScript.h"

void removeNotWriternote::exe(WListFast<WString> &ref)
{
    int i;
    const auto ext = qstr("." APP_EXT);

    for(i = 0; i < ref.size(); i++){
        if(ref.at(i).indexOf(ext) == -1){
            ref.remove(i);
            i--;
        }
    }
}
