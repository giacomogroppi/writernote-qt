#include "WZipWriterMulti.h"
#include "testing/memtest.h"

WZipWriterMulti::WZipWriterMulti(const char *fileZip, const char *fileInZip,
                            int thread, size_t *seek, size_t sizeFileInZip):
    _data(sizeFileInZip)
{
    int i;

    this->_size_file        = sizeFileInZip;
    this->_thread           = thread;
    this->_zip              = new WZipWriter;
    this->_writer           = new WZipWriterSingle[thread];
    this->_is_err           = false;
    this->_nameFileInZip    = fileInZip;

#ifdef DEBUGINFO
    memset(_data.get_data(), 0, sizeFileInZip);
#endif

    this->set_zip(fileZip);
    this->set_seek(seek);
}

WZipWriterMulti::WZipWriterMulti(
        const char *fileZip,
        const char *fileInZip,
        int thread,
        size_t *seek,
        WZipWriterSingle &writer):
    _data(writer.get_data()),
    _thread(thread)
{
    int i;

    this->_writer           = new WZipWriterSingle[thread];
    this->_is_err           = false;
    this->_nameFileInZip    = fileInZip;

    _writer[0].init(_data.get_data(), 0, seek[0]);
    for(i = 1; i < thread; i++){
        _writer[i].init(this->_data.get_data(), seek[i-1], seek[i]);
    }

    this->_zip = fileZip ? new WZipWriter : NULL;
    if(fileZip){
        this->set_zip(fileZip);
    }

    this->set_seek(seek);
}

void WZipWriterMulti::set_zip(const char *fileZip)
{
    if(this->_zip->init(fileZip)){
        this->set_err();
        return;
    }
}

void WZipWriterMulti::set_seek(size_t *seek)
{
    int i;
    for(i = 0; i < this->_thread - 1; i++){
        this->_writer[i].init( static_cast<char*>(_data.get_data()), seek[i], seek[i + 1] );
    }
    this->_writer[_thread - 1].init( (char *)_data.get_data(), seek[i], this->_size_file);
}

WZipWriterMulti::~WZipWriterMulti()
{
    delete      _zip;
    delete []   _writer;
}

/*
 * the save function writes the data to file, commits and then closes the file
*/
int WZipWriterMulti::commit()
{
    this->_zip->write(
                this->_data.get_data(),
                this->_size_file,
                this->_nameFileInZip
                );

    return 0;
}
