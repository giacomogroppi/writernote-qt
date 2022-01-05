#include "multi_thread_data.h"
#include "utils/common_script.h"

int DataPrivateMuThreadInit(DataPrivateMuThread *data, cint len, cint to)
{
    int i, done, div, count;

    done = 0;
    div = div_diff(to, len);

    if(to > len){
        count = len;

        for(i = 0; i < len; i++){
            data[i].from = done;
            data[i].to = done + div;
            done += div;
        }

        data[len - 1].to = to;
    }else{
        count = 1;
        data[0].from = 0;
        data[0].to = to;
    }

    return count;
}
