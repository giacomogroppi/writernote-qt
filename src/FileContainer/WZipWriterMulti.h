#pragma once

#include "FileContainer/WZipWriterSingle.h"
#include "FileContainer/WZipWriter.h"

class WZipWriterDataPrivate{
private:
    void *_data;
    bool  _allocated;

public:
    explicit WZipWriterDataPrivate(void *data);
    WZipWriterDataPrivate(size_t size);
    ~WZipWriterDataPrivate();

    [[nodiscard]] bool is_allocated() const;
    char *get_data();
};

class WZipWriterMulti {
private:
    WZipWriterDataPrivate   _data;
    WZipWriterSingle        *_writer;
    WZipWriter              *_zip;
    int                     _thread;
    bool                    _is_err;
    const char              *_nameFileInZip;
    size_t                  _size_file;

    void set_err();

    void set_zip(const char *fileZip);
    void set_seek(size_t *seek);

public:
    /*
     * \param seek è un array di distanze e ogni posizione segna la distanza
     * rispetto al primo byte in cui il thread va a scrivere
     * */
    WZipWriterMulti(const char *fileZip, const char *fileInZip,
                    int thread, size_t *seek);
    WZipWriterMulti(const char *fileZip, const char *fileInZip,
                    int thread, size_t *seek, WZipWriterSingle &writer);
    ~WZipWriterMulti();

    [[nodiscard]] WZipWriterSingle *get_writer(int id);
    [[nodiscard]] bool is_err() const;
#ifdef DEBUGINFO
    [[nodiscard]] size_t get_last_offset() const;
#endif // DEBUGINFO

    int commit();
};

inline WZipWriterSingle *WZipWriterMulti::get_writer(int id)
{
    W_ASSERT(id >= 0);
    W_ASSERT(id < this->_thread);
    return &_writer[id];
}

inline void WZipWriterMulti::set_err()
{
    W_ASSERT(this->_is_err == false);
    this->_is_err = true;
}

inline bool WZipWriterMulti::is_err() const
{
    return this->_is_err;
}

inline bool WZipWriterDataPrivate::is_allocated() const
{
    W_ASSERT(this->_data);
    return this->_allocated;
}

inline char *WZipWriterDataPrivate::get_data()
{
    W_ASSERT(_data);
    return static_cast<char *>(this->_data);
}

inline WZipWriterDataPrivate::WZipWriterDataPrivate(void *data)
{
    W_ASSERT(data);
    this->_data = data;
    this->_allocated = false;
}

inline WZipWriterDataPrivate::WZipWriterDataPrivate(size_t size)
{
    this->_data = WMalloc(size);
    this->_allocated = true;
}

inline WZipWriterDataPrivate::~WZipWriterDataPrivate()
{
    if(_allocated)
        WFree(_data);
}
