#include "struct_file.h"

#ifdef CLOUD

struct_file * bytearray_tostruct_file(WByteArray &array)
{
    if(array.size() != sizeof(struct struct_file))
        return NULL;

    struct_file * m_temp;
    m_temp = new struct_file;

    memcpy(m_temp, array.data(), sizeof(struct struct_file));

    return m_temp;

}

#endif // CLOUD