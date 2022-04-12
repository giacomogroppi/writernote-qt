#include "page_index_cache.h"
#include "touch/datastruct/page.h"
#include "iostream"
#include "utils/common_script.h"

// this module is not included in version 3.0.2h

// return 1 if element1 > element2
int function_for_check(const stroke &element1, const stroke &element2)
{
    const auto rect1 = element1.getBiggerPointInStroke();
    const auto rect2 = element2.getBiggerPointInStroke();

    return (int)(rect1.topLeft().y() > rect2.topLeft().y());
}

void page_index_cache::setAndAppend(qint32 index, float yMin) const
{
    auto &list = (QVector<page_index_internal> &)this->_index;

    const page_index_internal page = {
            ._index = index,
            ._yMin = (float) yMin,
    };

    list.append(page);
}

// this function assume that the list in page is order
int page_index_cache::find_without_cache(cdouble yMin) const
{
    int i, len;

    len = _page->lengthStroke();
    i = 0;

    W_ASSERT(this->_index.isEmpty());
    W_ASSERT(is_order_complex(_page->_stroke, function_for_check));

    for(; i < len;){
        const auto &stroke = _page->atStroke(i);
        const auto rect = stroke.getBiggerPointInStroke();

        if(is_included(yMin, (double)rect.topLeft().y(), (double) rect.bottomRight().y())){
            this->setAndAppend(i, (float)rect.bottomRight().y());

            return i;
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
