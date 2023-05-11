#include "WFile.h"
#include "utils/WCommonScript.h"
#include <filesystem>
#include <fstream>

WFile::WFile(const WByteArray &path, const char mode)
{
    char tmp[2] = {mode, '\0'};
    fp = fopen(path.constData(), tmp);
}

WFile::WFile(const std::string &path, const char mode):
    WFile(path.c_str(), mode)
{
}

WFile::WFile(const char *path, const char mode):
    WFile(WByteArray(path), mode)
{
}

WFile::~WFile()
{
    if(this->fp)
        fclose(this->fp);
}

int WFile::write(const void *data, size_t size)
{
    W_ASSERT(this->fp);
    const auto res = fwrite(data, size, 1, this->fp);
    //std::ofstream stream("ciao", std::ios_base::binary)

    if(un(res != 1))
        return -1;

    return 0;
}

int WFile::fileExist(const WByteArray &to)
{
    using namespace std;
    using namespace std::filesystem;

    if (exits(to.toStdString()))
        return 0;
    return -1;
}

int WFile::readFile(WByteArray &to, const char *pathFile)
{
    FILE *fp = WFile::open(pathFile, "r");

    if(!fp)
        return -1;

    const auto size = WFile::size(fp);

    char data[size];

    if(fread(data, size, 1, fp) < 1)
        return -1;

    to = WByteArray(data, size);

    return 0;
}

int WFile::saveArrIntoFile(const WByteArray &arr, const std::string &path)
{
    WFile file(path, 'w');

    if(!file.isValid())
        return -1;

    if(file.write(arr.constData(), arr.size()) < 0)
        return -2;

    return 0;
}
