#include "removenotwriternote.h"
#include <QList>
#include <QString>
#include "utils/common_script.h"

void removeNotWriternote::exe(QList<QString> &ref)
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
