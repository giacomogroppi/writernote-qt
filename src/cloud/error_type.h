#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

/*
 * common error that we can resive from the server
*/
namespace n_error_cloud {
    enum e_error_cloud{
        error_no_internet = 0,
        not_login = 1,
        not_register = 2,
        password_wrong = 3,
        ok = 4

    };
}


#endif // ERROR_TYPE_H
