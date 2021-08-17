#include "readlistarray.h"
#include "xmlstruct.h"

uchar readListArray::read(const QStringList &list_name,
                          zip_t *zip_file,
                          QList<QByteArray> &list_arr, const bool clear)
{
    QByteArray arr;
    uint i;

    if(clear)
        list_arr.clear();

    for(i=0; i<(uchar)list_name.length(); ++i){
        arr.clear();
        const QString &ref = list_name.at(i);
        if(xmlstruct::readFile(zip_file, arr, false, ref, false) != OK)
            return ERROR;
        list_arr.append(arr);
    }

    return OK;
}
