#include "toencrypt.h"
#include "stdio.h"

#ifdef CLOUD

#include <QCryptographicHash>

void toencrypy::getEcrypt(const WByteArray &arr, WByteArray &res)
{
    QCryptographicHash dd(QCryptographicHash::Algorithm::Keccak_512);

    res.clear();
    dd.addData(arr);
    res = dd.result();
}

#endif //CLOUD
