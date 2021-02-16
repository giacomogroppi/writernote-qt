#include <QStringList>

bool check1(QStringList *array){
    int i, lunghezza = array->length();
    for (i=1; i < lunghezza; i++)
        if((array->at(i).length() <= array->at(i-1).length())
                || array->at(i)[array->at(i).length() - 1] == ' ')
            return false;

    return true;
}


