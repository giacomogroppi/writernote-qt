#ifndef CHAR_TO_INT_CPP
#define CHAR_TO_INT_CPP

#define MAXSTRNUMERO 20
#include <iostream>
#include <string.h>
#include <math.h>

int chartoint(const char *numerochar){
    int number = 0, i, lunghezza = strlen(numerochar);

    for(i = 0; i < lunghezza; i++)
        number += (*(numerochar + i)-'0') * (int)pow ((float)10, (float)(lunghezza-i-1));

    return number;
}

void inttochar(int number, char numerostringa[]){
    int cicli, i;

    if (!number)
    {
        *numerostringa = '0';
        *(numerostringa + 1) = '\0';
        return;
    }

    if(number == 1)
    {
        *numerostringa = '1';
        *(numerostringa + 1) = '\0';
        return;
    }

    for(cicli=1, i = 0; cicli <= number; cicli *= 10, i++);

    i--;
    cicli /= 10;

    for(i=0; cicli >= 1; cicli /= 10, i++){
      numerostringa[i] = (char)(number/cicli) + '0';
      number -= (number/cicli)*cicli;
    }
    numerostringa[i] = '\0';

}

#endif //CHAR_TO_INT_CPP
