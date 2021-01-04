#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#define POSIZIONEPATHVERSIONE ":/image/src/windows/versione.xml"
#define SITOGIT "https://api.github.com/repos/giacomogroppi/writernote-qt/contents/src/windows/versione.xml"

#define NOMECONTENT "\"content\""
#define NOMEFINE "encoding"

class updatecheck
{
public:
    updatecheck();
    int currentversione;

    bool checkupdate();

};

#endif // UPDATECHECK_H
