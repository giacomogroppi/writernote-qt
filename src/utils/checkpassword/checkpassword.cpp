#include "checkpassword.h"

#include "string.h"

int checkpassword(const char *first, const char *second)
{
    if(strcmp(first, second) != 0)
        return PASS_NOT_S;

    int i, numm, numn;

    for(i=0, numm = numn = 0; first[i] != '\0'; i++){
        if(first[i] >= 'A' and first[i] <= 'Z')
            numm ++;

        else if(first[i] >= '0' and first[i] <= '9')
            numn ++;
    }


    if(numm < MIN_NUMM)
        return PASS_M;

    if(numn < MIN_NUMN)
        return PASS_N;

    if(i<MINSTRLEN)
        return PASS_LEN;

    return OK_PASSWORD;
}
