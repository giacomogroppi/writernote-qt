#include "datastruct.h"
#include "log/log_ui/log_ui.h"

/*
 * the point at the IndexPoint
 * position is not included
 *
 * after this function you have to draw the image of
 * the page. Passing as a parameter all true
*/
void datastruct::changeId(int IndexPoint, int indexStroke, int indexPage, int newId)
{
    page &page = at_mod(indexPage);
    stroke &stroke = page.atStrokeMod(indexStroke);

    Q_ASSERT(newId >= 0);

    changeId(IndexPoint, stroke, at_mod(indexPage), newId);
}

void datastruct::changeId(int IndexPoint, stroke &__stroke, page &page, int newId)
{
    stroke strokeToAppend;
    int lenPointInStroke = __stroke.length();

    strokeToAppend.reset();

    qDebug() << "datastruct::changeId start" << IndexPoint << __stroke.length()
             << __stroke.last().m_x << __stroke.last().m_y;

    Q_ASSERT(newId >= 0);

    for(int secIndex = IndexPoint; secIndex < lenPointInStroke; secIndex ++){
        strokeToAppend.append(__stroke.at(secIndex));
    }

    __stroke.removeAt(IndexPoint, __stroke.length() - 1);

    strokeToAppend.setMetadata(__stroke.getMetadata());
    strokeToAppend.setId(newId);

    page.append(strokeToAppend);
}
