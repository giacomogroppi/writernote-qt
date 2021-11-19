#include "removenotwriternote.h"

void removeNotWriternote::exe(QStringList &ref)
{
    int i;
    for(i=0; i<ref.length(); ++i){
        if(ref.at(i).indexOf('.' + APP_EXT) == -1){
            ref.removeAt(i);
            i--;
        }
    }
}
