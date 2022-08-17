#include "wzip.h"
#include "testing/memtest.h"

WZip::WZip(const QByteArray &path, bool &ok)
{
    ok = this->openZip(path);
}

WZip::~WZip()
{
#ifdef DEBUG_INFO
    if(!this->_zip)
        W_ASSERT(!this->_file);
#endif

    if(this->_file)
        zip_fclose(this->_file);
    if(this->_zip)
        zip_fclose(this->_zip);

    DO_IF_DEBUG(this->_file = NULL;)
    DO_IF_DEBUG(this->_zip  = NULL;)
}

zip_t *WZip::openZipWrite(const QByteArray &path)
{
    const int flag = ZIP_CREATE;
    const char *pos = path.constData();
    return zip_open(pos, flag, nullptr);
}

bool WZip::openZip(const QByteArray &path)
{
    W_ASSERT(!path.isEmpty());
    W_ASSERT(this->_zip == NULL);
    W_ASSERT(this->_file == NULL);

    int err;
    int flag = 0;
    const char *nameFile = path.constData();

    flag |= ZIP_RDONLY;

    _zip = zip_open(nameFile, flag, &err);

    if(_zip){
        this->_status.set_zip_open();
    }
    return !!_zip;
}

bool WZip::openFileInZip(const QByteArray &path)
{
    W_ASSERT(_zip);
    W_ASSERT(this->_status.is_zip_open());

    _file = zip_fopen(_zip, path.constData(), 0);

    return !!_file;
}

#define reset_all() do {    \
    zip_fclose(_file);      \
    zip_close(_zip);        \
    _file = NULL;           \
    _zip = NULL;            \
    }while(0);


bool WZip::alloc_and_close(const char *nameFile)
{
    W_ASSERT(nameFile);
    W_ASSERT(this->_zip);
    W_ASSERT(this->_file);
    W_ASSERT(this->_status.is_file_open());
    W_ASSERT(this->_status.is_zip_open());
    const auto size = WZip::get_size_file(_zip, nameFile);
    this->_data = WMalloc(size);

    this->_status.set_data_available();
    this->_status.set_file_open();
    this->_status.set_zip_open();

    if(size != zip_fread(_file, _data, size)){
        WFree(_data);
        _data = NULL;

        reset_all();
        return false;
    }

    reset_all();

    return true;
}
