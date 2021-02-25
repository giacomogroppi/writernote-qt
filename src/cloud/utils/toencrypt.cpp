#include "toencrypt.h"
#include "stdio.h"

#ifdef CLOUD

#include "crypto++/cryptlib.h"
#include "crypto++/modes.h"
#include "crypto++/filters.h"
#include "crypto++/aes.h"
#include "crypto++/osrng.h"
#include "crypto++/strciphr.h"

#include <iostream>
#include <iomanip>

#include "cryptopp/base64.h"


static const char * SHA256HashString(std::string );

const char * toencrypt(const char *data)
{
    return SHA256HashString((std::string)data);
}


static const char * SHA256HashString(std::string aString){
    std::string digest;
    CryptoPP::SHA256 hash;


    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    char * temp = new char [digest.length()];

    memcpy(temp, digest.c_str(), sizeof(char)*digest.length());

    return temp;
}

#endif //CLOUD
