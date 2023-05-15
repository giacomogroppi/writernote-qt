#include "readlistarray.h"
#include "xmlstruct.h"

uchar readListArray::read(const WListFast<QString> &list_name,
                          zip_t *zip_file,
                          WListFast<QByteArray> &list_arr, cbool clear)
{
    QByteArray arr;
    unsigned i;

    if(clear)
        list_arr.clear();

    for(i = 0; i < list_name.length(); i++){
        arr.clear();
        const QString &ref = list_name.at(i);
        if(xmlstruct::readFile(zip_file, arr, false, ref, false) != OK)
            return ERROR;
        list_arr.append(arr);
    }

    return OK;
}

unsigned char readListArray::read(const WListFast<QString> &list_name, WZip &zip,
                                  WListFast<QByteArray> &list_arr, cbool clear)
{
    QByteArray arr;
    unsigned i;
    auto *zipFile = zip.get_zip();

    if(clear)
        list_arr.clear();

    for(i = 0; i < list_name.length(); i++){
        arr.clear();
        const QString &ref = list_name.at(i);

        if(xmlstruct::readFile(zipFile, arr, false, ref, false) != OK)
            return ERROR;

        list_arr.append(arr);
    }

    return OK;
}
