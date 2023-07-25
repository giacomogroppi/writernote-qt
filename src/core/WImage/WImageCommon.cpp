#include "WImage.h"

size_t WImage::get_size_in_file() const
{
    WByteArray arr;
    const auto s = this->save_and_size(arr);
    return s;
}
