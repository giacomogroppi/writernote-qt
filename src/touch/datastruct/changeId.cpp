#include "datastruct.h"
#include "../../log/log_ui/log_ui.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(int IndexPoint, int indexStroke, int indexPage, int newId)
{
    stroke strokeToAppend;
    page &page = at_mod(indexPage);
    const stroke &stroke = at(indexPage).atStroke(indexStroke);
    int lenPointInStroke = stroke.length();

    if(newId == -1)
        newId = maxId() + 1;

    for(; IndexPoint < lenPointInStroke; IndexPoint ++){
        strokeToAppend.append(stroke.at(IndexPoint));
    }

    for(; lenPointInStroke >= IndexPoint; lenPointInStroke --){
        page.atStrokeMod(indexStroke).removeAt(lenPointInStroke);
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
