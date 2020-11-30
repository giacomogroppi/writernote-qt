#ifndef STRING_GENERATOR_CPP
#define STRING_GENERATOR_CPP
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <QDebug>

const char * stringgenerator(const int numerolettere){
    std::string tmp_s;
       static const char alphanum[] =
           "0123456789"
           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
           "abcdefghijklmnopqrstuvwxyz";

       srand( (unsigned) time(NULL) * getpid());

       for (int i = 0; i < numerolettere; ++i)
           tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


       return tmp_s.c_str();
}

#endif
