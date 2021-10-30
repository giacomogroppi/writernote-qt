#include "toencrypt.h"
#include "stdio.h"

#ifdef CLOUD

#include <QCryptographicHash>

void toencrypy::getEcrypt(const QByteArray &arr, QByteArray &res)
{
    QCryptographicHash dd(QCryptographicHash::Algorithm::Keccak_512);

    res.clear();
    dd.addData(arr);
    res = dd.result();
}

#endif //CLOUD
