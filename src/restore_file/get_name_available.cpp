#include "get_name_available.h"
#include "../utils/common_error_definition.h"
#include "../dataread/xmlstruct.h"

QString get_name_available::get(const QString &path, bool &ok, Document *doc)
{
    uint i, len;
    indice_class ind;
    const bool needDelete = doc == nullptr;
    if(needDelete)
        doc = new Document;
    else
        doc->reset();

    xmlstruct xml(path, ind, *doc);

    if(!xml.loadindice()){
        ok = false;
        return "";
    }

    len = ind.titolo.length();

    for(i=0; i<len; ++i){
        /* there is only one copybook that load without error */
        if(xml.loadfile(ind.titolo.at(i)+".xml", false, false) != ERROR){
            ok = true;
            if(needDelete)
                delete doc;

            return ind.titolo.at(i);
        }
    }

    ok = false;
    return "";
}
