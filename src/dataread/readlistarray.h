#pragma once

#include "core/WByteArray.h"
#include "core/WList.h"
#include "utils/common_error_definition.h"
#include "FileContainer/WZip.h"
#include "core/WString.h"
#include "core/WListFast.h"

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
     * alla lista di WByteArray
*/
    static unsigned char read(const WListFast<WString> &list_name, FileContainer *zip_file,
                     WListFast<WByteArray> &arr, cbool clear);

    static unsigned char read(const WListFast<WString> &list_name, WZip &zip,
                      WListFast<WByteArray> &arr, cbool clear);
};


