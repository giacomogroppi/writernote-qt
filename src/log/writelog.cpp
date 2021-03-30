#include "controll_log.h"
#include "stdio.h"
#include <QFile>
#include "../utils/permission/permission.h"
#include "../utils/areyousure/areyousure.h"
#include "../utils/dialog_critic/dialog_critic.h"


#define pos(string) string.toUtf8().constData()

#define CriticError "PANIC: "
#define Info "Info: "
#define Error "Error: "


#define controllEnable(x) if(x != permi::enable) \
    return;

void controll_log::write(QString &string, controll_log::type_write val){
    if(this->m_permi == permi::error){
        QString temp;
        e_setOut res = this->setOut(temp);
        if(res == e_setOut::ref){
            this->m_permi = permi::disable;
        }
    }

    controllEnable(m_permi);



}

void controll_log::write(const char *string, controll_log::type_write val){
    controllEnable(m_permi);
}

void controll_log::write(QString string, controll_log::type_write val){
    controllEnable(m_permi);



}


controll_log::errorWriting controll_log::manageWrite(const char *fileName, QString &content, controll_log::type_write val){
    errorWriting res;

    if(val == controll_log::info)
        res = write_(fileName, pos((content + Info)));
    else if(val == controll_log::error_internal)
        res = write_(fileName, pos((content + Error)));
    else
        res = write_(fileName, pos((content + CriticError)));

    return res;

}

controll_log::errorWriting controll_log::write_(const char *fileName, const char *content){
    if(!QFile::exists(fileName))
        return errorWriting::not_exist;


    FILE *fp;
    fp = fopen(fileName, "a");

    if(!fp)
        return errorWriting::error_append;

    fprintf(fp, "%s", content);

    fclose(fp);
    return errorWriting::ok_writing;
}

bool controll_log::ovverrideError(errorWriting res){
    FILE *fp;
    int i = 0;

    if(res == errorWriting::error_append){
        if(areyousure(nullptr, "Error write log", "Do you want to ovveride the log file locate in " + pos + "?")){
            goto test_file;
        }
        else
            return false;
    }


    test_file:
    fp = fopen(pos(pos), "w");
    if(!fp)
        goto error_write;
    messaggio_utente("Ok, the file is opening");
    fclose(fp);

    return true;


    error_write:
    if(i>1){
        /* if we already try one time to ovveride the file */
        QString temp;
        auto res = this->setOut(temp);
        if(res == e_setOut::ref)
            return false;
        pos = temp;
        goto test_file;

    }
    else if(areyousure(nullptr,"Error ovveride file", "We had a problem ovverride the file locate in " + pos + "\nDo you want to change location")){
        ++i;
        goto test_file;
    }
    return false;

}



