#ifndef READLISTARRAY_H
#define READLISTARRAY_H

#include "zip.h"
#include <QByteArray>
#include <QList>
#include <QStringList>
#include "../utils/common_error_definition.h"

class readListArray
{
public:
    /*
     * list_name: all the names of the file to open
     * zip_file: name of the zip file
     * arr: the array to write
     * it returns ERROR if it encountered an error
     *
     * la funzione serve per leggere sia i pdf che le immagini
     * bisogna passargli un filezip già aperto, e lui provvederà ad aprire
     * ogni file, per ogni nome all'interno del list_name e apperderà tutto
     * alla lista di QByteArray
*/
    static uchar read(const QStringList &list_name, zip_t *zip_file,
                     QList<QByteArray> &arr, const bool clear);
};

#endif // READLISTARRAY_H
