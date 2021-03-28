#include "controll_log.h"
#include "stdio.h"
#include <QFile>
#include "../utils/permission/permission.h"
#include "../utils/areyousure/areyousure.h"

enum errorWriting: unsigned int{
    ok,
    error_append,
    not_exist

};

static errorWriting write(const char *, const char *);
#define controllEnable(x) if(x == permi::disable) \
    return;

void controll_log::write(QString &string){
    controllEnable(m_permi)

    if(this->m_permi == permi::error){
        QString temp;
        e_setOut res = this->setOut(temp);
        if(res == e_setOut::ref){
            this->m_permi = permi::disable;
        }
    }
}

void controll_log::write(const char *string){
    controllEnable(m_permi);
}

void controll_log::write(QString string){
    controllEnable(m_permi);
}

static errorWriting write(const char *fileName, const char *content){
    if(!QFile::exists(fileName))
        return errorWriting::not_exist;


    FILE *fp;
    fp = fopen(fileName, "a");

    if(!fp)
        return errorWriting::error_append;

    fprintf(fp, "%s", content);

    fclose(fp);
    return errorWriting::ok;
}
