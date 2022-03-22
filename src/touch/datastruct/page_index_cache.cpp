#include "page_index_cache.h"
#include "touch/datastruct/page.h"
#include "iostream"
#include "stdlib.h"

// return 1 if element1 > element2
int function_for_check(const stroke &element1, const stroke &element2)
{
    const auto rect1 = element1.getBiggerPointInStroke();
    const auto rect2 = element2.getBiggerPointInStroke();

    return (int)(rect1.topLeft().y() > rect2.topLeft().y());
}

// this function assume that the list in page is order
int page_index_cache::find_without_cache(cdouble y) const
{
    int i, len;

    len = _page->lengthStroke();
    W_ASSERT(is_order_complex(_page->_stroke, function_for_check));

    for(i = 0; i < len; i++){
        const auto &stroke = _page->atStroke(i);
        const auto rect = stroke.getBiggerPointInStroke();
        if(rect.topLeft().y() > y){

        }
    }

}

int page_index_cache::index_stroke(
                cdouble Y,
                int &from,
                int &to) const
{
    int i, len;

    len = _index.length();

    if(this->is_all_null()){
        return find_without_cache(Y);
    }

    for(i = 0; i < len; i++){

    }
}

force_inline bool page_index_cache::is_all_null() const
{
    int i, len;

    len = _index.length();

    for(i = 0; i < len; i++){
        if(unlikely(_index[i]._index != page_index_cache::null_val)){
            return true;
        }
    }

    return false;
}
