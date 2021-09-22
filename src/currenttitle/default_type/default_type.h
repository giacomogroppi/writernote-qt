#ifndef DEFAULT_TYPE_H
#define DEFAULT_TYPE_H

class default_type
{
public:
    enum e_type_copybook: int{
        pen,
        key
    };
    static enum e_type_copybook load_preference_copybook();
    static void save_preference_copybook(enum e_type_copybook);

};

#endif // DEFAULT_TYPE_H
