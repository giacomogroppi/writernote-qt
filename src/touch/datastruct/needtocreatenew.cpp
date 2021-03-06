#include "datastruct.h"

/*
 * the function return true if we need to create a new sheet
 */

bool datastruct::needtocreatenew(){

    if(m_point.isEmpty() || posizionefoglio.isEmpty())
        return true;


    int i, len;
    len = m_point.length();

    double maxy = m_point.first().m_y;

    for(i=0; i<len; i++){
        if(m_point.at(i).m_y > maxy
                && m_point.at(i).idtratto != IDVERTICALE
                && m_point.at(i).idtratto != IDORIZZONALE)
            maxy = m_point.at(i).m_y;
    }

    return maxy > posizionefoglio.last();
}
