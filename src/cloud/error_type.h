#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

#define PRIVATE_CLOUD_ERROR_NO_INTERNET 0
#define PRIVATE_CLOUD_NOT_LOGIN 1
#define PRIVATE_CLOUD_NOT_REGISTER 2
#define PRIVATE_CLOUD_PASSWORD_WRONG 3
#define PRIVATE_CLOUD_OK 4
#define PRIVATE_CLOUD_SERVER_DOWN 5
#define PRIVATE_WAIT 6


/*
 * common error that we can resive from the server
*/
namespace n_error_cloud {
    enum e_error_cloud{
        error_no_internet = PRIVATE_CLOUD_ERROR_NO_INTERNET,
        not_login = PRIVATE_CLOUD_NOT_LOGIN,
        not_register = PRIVATE_CLOUD_NOT_REGISTER,
        password_wrong = PRIVATE_CLOUD_PASSWORD_WRONG,
        ok = PRIVATE_CLOUD_OK,
        server_down = PRIVATE_CLOUD_SERVER_DOWN,
        wait = PRIVATE_WAIT
    };
}


#endif // ERROR_TYPE_H
