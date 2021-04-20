#ifndef ITSPOSSIBLETOSCROLL_H
#define ITSPOSSIBLETOSCROLL_H

class datastruct;


class scroll{

public:
    static bool itspossibletoscrolly(datastruct *data, short int height, double *__pos_delta);
    static bool itspossibletoscrollx(datastruct *data, short int width, double *__pos_delta);


};


#endif // ITSPOSSIBLETOSCROLL_H
