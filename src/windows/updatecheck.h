#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#define POSIZIONEPATHVERSIONE ":src/windows/versione.xml"

class updatecheck
{
public:
    updatecheck();
    int currentversione;

    bool checkupdate();

};

#endif // UPDATECHECK_H
