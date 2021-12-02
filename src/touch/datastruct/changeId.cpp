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
    stroke *stroke = &page.atStrokeMod(indexStroke);
    int lenPointInStroke = stroke->length();

    strokeToAppend.reset();

    qDebug() << "datastruct::changeId start" << IndexPoint << stroke->length() << stroke->last().m_x << stroke->last().m_y;

    if(newId < 0)
        newId = maxId() + 1;

    for(int secIndex = IndexPoint; secIndex < lenPointInStroke; secIndex ++){
        strokeToAppend.append(stroke->at(secIndex));
    }

    stroke->removeAt(IndexPoint, stroke->length() - 1);

    strokeToAppend.setMetadata(stroke->getMetadata());
    strokeToAppend.setId(newId);

    if(stroke->getColor().alpha() != 255){
        /* we need to append to the top */
        page.appendToTheTop(strokeToAppend);
    }else{
        page.AppendDirectly(strokeToAppend);
    }

    qDebug() << "datastruct::changeId end" << IndexPoint << stroke->length() << stroke->last().m_x << stroke->last().m_y;

}
