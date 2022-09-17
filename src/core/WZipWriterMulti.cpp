#include "WZipWriterMulti.h"
#include "testing/memtest.h"

WZipWriterMulti::WZipWriterMulti(const char *fileZip, const char *fileInZip,
                            int thread, size_t *seek, size_t sizeFileInZip)
{
    int i;

    this->_data             = WMalloc(sizeFileInZip);
    this->_size_file        = sizeFileInZip;
    this->_thread           = thread;
    this->_zip              = new WZipWriter;
    this->_writer           = new WZipWriterSingle[thread];
    this->_is_err           = false;
    this->_nameFileInZip    = fileInZip;

#ifdef DEBUGINFO
    memset(_data, 0, sizeFileInZip);
#endif

    if(this->_zip->init(fileZip)){
        this->set_err();
        return;
    }

    for(i = 0; i < thread - 1; i++){
        this->_writer[i].init( static_cast<char*>(_data), seek[i], seek[i + 1] );
    }
    this->_writer[thread - 1].init( (char *)_data, seek[i], sizeFileInZip);
}

WZipWriterMulti::~WZipWriterMulti()
{

}

/*
 * the save function writes the data to file, commits and then closes the file
*/
int WZipWriterMulti::commit()
{
    this->_zip->write(
                this->_data,
                this->_size_file,
                this->_nameFileInZip
                );

    WFree(this->_data);

    return 0;
}
