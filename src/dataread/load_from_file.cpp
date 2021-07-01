#include "load_from_file.h"


bool load_from_file::exe(QByteArray &arr, const QString &path, const bool clear)
{
    FILE *fp;
    uchar __read;

    if(clear){
        arr.clear();
    }

    fp = fopen(path.toUtf8().constData(), "r");

    if(!fp){
        return false;
    }

    do {
        fread(&__read, 1, 1, fp);
        arr.append(__read);
    }
    while(!feof(fp));

    fclose(fp);

    return true;
}
