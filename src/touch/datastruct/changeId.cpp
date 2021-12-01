#include "datastruct.h"
#include "../../log/log_ui/log_ui.h"

/*
 * the point at the IndexPoint
 * position is not included
*/
void datastruct::changeId(int IndexPoint, int indexStroke, int indexPage, int newId)
{
    stroke strokeToAppend;
    page &page = at_mod(indexPage);
    stroke &stroke = page.atStrokeMod(indexStroke);
    int lenPointInStroke = stroke.length();

    if(newId < 0)
        newId = maxId() + 1;

    for(; IndexPoint < lenPointInStroke;){
        strokeToAppend.append(stroke.at(IndexPoint));

        stroke.removeAt(IndexPoint);

        lenPointInStroke --;
    }

    strokeToAppend.setMetadata(stroke.getMetadata());

    strokeToAppend.setId(newId);

    if(stroke.getColor().alpha() != 255){
        /* we need to append to the top */
        page.appendToTheTop(strokeToAppend);

    }else{
        page.append(strokeToAppend);
    }
}
