#ifndef WRITERNOTE_QT_PAGE_INDEX_CACHE_H
#define WRITERNOTE_QT_PAGE_INDEX_CACHE_H

#include "utils/common_script.h"

// all functions are marked as const as we don't want to
// run the copy of the page just because their cache
// changes, but we want them to be shared as objects

class page_index_cache {
private:
    struct page_index_internal{
        qint32  _index;
        float   _yMin;
    };

    static constexpr int null_val = -1;

    void setAndAppend(qint32 _index, float _yMin) const;
    int find_without_cache(cdouble yMin) const;
    bool is_all_null() const;
    const class page *_page;
    QVector<page_index_internal> _index = {};
public:
    page_index_cache(const class page *page);
    ~page_index_cache() = default;

    void reset() const;
    int index_stroke(cdouble yMin, int &from, int &to) const;
    page_index_cache &operator=(const page_index_cache &other);
};

force_inline page_index_cache::page_index_cache(const class page *page)
{
    this->reset();
    this->_page = page;
}

force_inline page_index_cache &page_index_cache::operator=(const page_index_cache &other)
{
    if(unlikely(this == &other)){
        return *this;
    }

    this->_index    = other._index;
    this->_page     = other._page;

    return *this;
}

force_inline void page_index_cache::reset() const
{
    int i;
    int len = _index.length();
    void **parent = (void **) &_page;

    for(i = 0; i < len; i++){
        auto &index = (int &) _index[i]._index;
        index = null_val;
    }

    *parent = NULL;
}

#endif //WRITERNOTE_QT_PAGE_INDEX_CACHE_H
