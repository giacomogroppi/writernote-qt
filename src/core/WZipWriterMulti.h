#pragma once

#include "core/WZipWriterSingle.h"
#include "core/WZipWriter.h"

class WZipWriterMulti {
private:
    void                *_data;
    WZipWriterSingle    *_writer;
    WZipWriter          *_zip;
    size_t              _size_file;
    int                 _thread;
    bool                _is_err;

    void set_err();

public:
    /*
     * @seek è un array di distanze e ogni posizione segna la distanza
     * rispetto al primo byte in cui il thread va a scrivere
     * */
    WZipWriterMulti(const char *fileZip, const char *fileInZip,
                    int thread, size_t *seek, size_t sizeFileInZip);
    ~WZipWriterMulti();
    bool is_err() const;
};

inline void WZipWriterMulti::set_err()
{
    W_ASSERT(this->_is_err == false);
    this->_is_err = true;
}

inline bool WZipWriterMulti::is_err() const
{
    return this->_is_err;
}

