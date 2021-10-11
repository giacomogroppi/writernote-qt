#include "load_from_file.h"


bool load_from_file::exe(QByteArray &arr, const QString &path, const bool clear)
{
    FILE *fp;
    uchar __read;
    uchar size;
    bool error = true;

    if(clear){
        arr.clear();
    }

    fp = fopen(path.toUtf8().constData(), "r");

    if(!fp){
        return false;
    }

    while(1){
        size = fread(&__read, 1, 1, fp);

        if(feof(fp))
            break;

        if(!size){
            error = false;
            goto free;
        }

        arr.append(__read);
    }


    free:
    fclose(fp);
    return error;
}
