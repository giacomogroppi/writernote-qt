#include "readlistarray.h"
#include "xmlstruct.h"

unsigned char readListArray::read(const WListFast<WString> &list_name,
                          zip_t *zip_file,
                          WListFast<WByteArray> &list_arr, cbool clear)
{
    WByteArray arr;
    int i;

    if(clear)
        list_arr.clear();

    for(i = 0; i < list_name.size(); i++){
        arr.clear();
        const WString &ref = list_name.at(i);
        if(xmlstruct::readFile(zip_file, arr, false, ref, false) != OK)
            return ERROR;
        list_arr.append(arr);
    }

    return OK;
}

unsigned char readListArray::read(const WListFast<WString> &list_name, WZip &zip,
                                  WListFast<WByteArray> &list_arr, cbool clear)
{
    WByteArray arr;
    int i;
    auto *zipFile = zip.get_zip();

    if(clear)
        list_arr.clear();

    for(i = 0; i < list_name.size(); i++){
        arr.clear();
        const WString &ref = list_name.at(i);

        if(xmlstruct::readFile(zipFile, arr, false, ref, false) != OK)
            return ERROR;

        list_arr.append(arr);
    }

    return OK;
}
