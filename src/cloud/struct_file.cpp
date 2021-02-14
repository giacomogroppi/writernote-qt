#include "struct_file.h"

struct_file * bytearray_tostruct_file(QByteArray &array){
    if(array.size() != sizeof(struct struct_file) || !array.size())
        return NULL;

    struct_file * m_temp;
    m_temp = new struct_file;

    memcpy(m_temp, array.data(), sizeof(struct struct_file));

    return m_temp;

}
