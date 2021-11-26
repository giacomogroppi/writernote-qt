#include "datastruct.h"
#include "../../log/log_ui/log_ui.h"

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(uint indexStroke, uint indexPage, uint lenPage, int base)
{
    if(base == -1)
        base = maxId() + 1;

    if(lenPage == 0){
        lenPage = this->lengthPage();
        if(!lenPage)
            LOG("datastruct::changeId lenPage = 0", log_ui::type_write::possible_bug);
    }

    page &page = at_mod(indexPage);

    page.atStrokeMod(indexStroke).setId(base);
}
