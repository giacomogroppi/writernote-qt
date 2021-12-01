#include "datastruct.h"
#include "../../log/log_ui/log_ui.h"

/*
 * the point at the IndexPoint
 * position is not included
 *
 * after this function you have to draw the image of
 * the page. Passing as a parameter all true
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

    qDebug() << "datastruct::changeId" << IndexPoint;

    /* if the point has less than two points
     * it makes no sense to add it to the list,
     *  as it is not possible to draw a
     *  single point.
    */
    if(stroke.length() < 2)
        page.removeAt(indexStroke);

    if(strokeToAppend.length() < 2) return;

    if(stroke.getColor().alpha() != 255){
        /* we need to append to the top */
        page.appendToTheTop(strokeToAppend);

    }else{
        page.AppendDirectly(strokeToAppend);
    }
}
