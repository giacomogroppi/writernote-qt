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
/*static void removeIdUser(datastruct *data, QList<int>&list){


}*/

void datastruct::reorganize()
{

}
