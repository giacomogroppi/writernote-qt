#ifndef WRITERNOTE_QT_PAGE_INDEX_CACHE_H
#define WRITERNOTE_QT_PAGE_INDEX_CACHE_H

#include "utils/common_script.h"

// all functions are marked as const as we don't want to
// run the copy of the page just because their cache
// changes, but we want them to be shared as objects

class page_index_cache {
private:
    static constexpr int index_max = 10;

    class page *_page;
    int _index[index_max];
public:
    page_index_cache(const class page *page);
    ~page_index_cache() = default;

    void reset() const;
    int index_stroke(const class page &Page, cdouble Y) const;
    page_index_cache &operator=(const page_index_cache &other);
};

force_inline page_index_cache &page_index_cache::operator=(const page_index_cache &other)
{
    if(unlikely(this == &other)){
        return *this;
    }

    memcpy(this, &other, sizeof(*this));

    return *this;
}

force_inline void page_index_cache::reset() const
{
    memset((int *)&_index, 0, sizeof(_index));
}

#endif //WRITERNOTE_QT_PAGE_INDEX_CACHE_H
