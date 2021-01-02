#include "updatecheck.h"

#include "stdlib.h"
#include "stdio.h"

#include <cpr/cpr.h>

updatecheck::updatecheck()
{
    FILE *fp;
    fp = fopen(POSIZIONEPATHVERSIONE, "r");

    if(!fp){
        this->currentversione = -1;
        return;
    }

    if(!fscanf(fp, "%d", &this->currentversione)){
        this->currentversione = -1;

    }

    fclose(fp);

}

bool updatecheck::checkupdate(){
    if(this->currentversione == -1)
        return false;

    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    auto ciao = r.text;                         // JSON text string


    return true;
}
