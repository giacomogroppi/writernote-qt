#ifndef ITSPOSSIBLETOSCROLL_H
#define ITSPOSSIBLETOSCROLL_H

class datastruct;


class scroll{

public:
    static bool itspossibletoscrolly(datastruct *data,
                                     const short int height,
                                     double *__pos_delta);
    static bool itspossibletoscrollx(datastruct *data,
                                     const short int width,
                                     double *__pos_delta);


};


#endif // ITSPOSSIBLETOSCROLL_H
