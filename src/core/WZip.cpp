#include "WZip.h"
#include "testing/memtest.h"
#include "core/WZipCommon.h"

WZip::WZip(const QByteArray &path, bool &ok)
{
    this->_data = NULL;
    ok = this->openZip(path);
    if(ok){
        this->_status.set_zip_open();
        this->_status.set_data_not_available();
    }
}

void WZip::dealloc_file()
{
    W_ASSERT(this->_data);
    W_ASSERT(this->_status.is_data_available());
    WFree(this->_data);
    this->_data = NULL;
    this->_status.set_data_not_available();
}

WZip::~WZip()
{
#ifdef DEBUG_INFO
    if(!this->_zip)
        W_ASSERT(!this->_file);
#endif

    if(this->_zip){
        zip_close(this->_zip);
        this->_zip = nullptr;
    }

    DO_IF_DEBUG(this->_zip  = nullptr;)
}

bool WZip::openZip(const QByteArray &pathZip)
{
    W_ASSERT(!pathZip.isEmpty());
    W_ASSERT(this->_zip == nullptr);

    int err;
    int flag = 0;
    const char *nameFile = pathZip.constData();

    flag |= ZIP_RDONLY;

    _zip = zip_open(nameFile, flag, &err);

    if(_zip){
        this->_status.set_zip_open();
    }

    return _zip != nullptr;
}

zip_file_t *WZip::open_file_in_zip(const QByteArray &nameFile)
{
    zip_file_t *file;

    file = zip_fopen(_zip, nameFile.constData(), 0);

    return file;
}

// return true on success
bool WZip::openFileInZip(const QByteArray &nameFile)
{
    W_ASSERT(_zip);
    W_ASSERT(this->_status.is_zip_open());

    zip_file_t *file;

    this->_len_file = WZip::get_size_file(_zip, nameFile.constData());
    this->_data = WMalloc(_len_file);

    file = open_file_in_zip(nameFile);

    if(!file)
        return false;

    if(zip_fread(file, _data, _len_file) != _len_file){
        zip_fclose(file);
        WFree(_data);
        _data = nullptr;

        return false;
    }

    zip_fclose(file);
    return true;
}
