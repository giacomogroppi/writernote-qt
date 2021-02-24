#include "toencrypt.h"
#include "stdio.h"

#ifdef CLOUD

#include "crypto++/cryptlib.h"
#include "crypto++/sha.h"
#include "crypto++/hex.h"

#include <iostream>
#include <iomanip>

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

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
