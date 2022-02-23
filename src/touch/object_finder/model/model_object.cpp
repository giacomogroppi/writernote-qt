#include "model.h"
#include "touch/datastruct/datastruct.h"

constexpr double max_segno_var = 0.01;

bool model_line(const stroke *stroke)
{
    int segno_var = 0;
    int i, len;
    const point_s *one, *two;
    const auto &area = stroke->getBiggerPointInStroke();

    len = stroke->length();

    if(unlikely(!len)){
        return false;
    }

    one = &stroke->at(0);

    for(i = 1; i < len; i++){
        two = & stroke->at(i);

        if(two->x() * one->x() < 0){
            segno_var ++;
        }
        if(two->y() * one->y() < 0){
            segno_var ++;
        }

        one = two;
    }

}
