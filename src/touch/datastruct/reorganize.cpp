#include "datastruct.h"

/* the function restores the ids, as
 * there may be holes, when we go
 * to eliminate some strokes written
 * with the pen
*/
struct disponibili{
    int current; /* current id */
    int move; /* id to change */
};

/*
 * the function remove all the id used by the system
*/
static void removeIdUser(datastruct *data, QList<int>&list){
    uint i, len;

    len = list.length();

    for(i=0; i<len; ++i){
        if(!datastruct::isIdUser(list.at(i)))
            list.removeAt(i);
    }

}

void datastruct::reorganize()
{
    uint i, len, k;
    int _lastId, id;
    len = length();

    if(!len)
        return;

    const point_s * __point = firstPoint();


    _lastId = __point->idtratto;

    QList<int> _list_id_used, _list_disponibili;

    for(i=0; i<len; ++i){
        __point = at(i);
        if(_list_id_used.indexOf(__point->idtratto) == -1){
            _list_id_used.append(__point->idtratto);
        }
        moveNextPoint(&i, len, __point->idtratto);
    }

    removeIdUser(this, _list_id_used);


    len = _list_id_used.length();
    for(i=0; i<len-1; ++i){
        id = _list_id_used.at(i);
        if((id + 1) != _list_id_used.at(i+1)){

            for(; (id + 1) != _list_id_used.at(i); ++id){
                _list_disponibili.append(id);
            }

            _list_disponibili.append(id);
        }
    }


}
