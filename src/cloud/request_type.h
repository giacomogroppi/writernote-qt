#ifndef REQUEST_TYPE_H
#define REQUEST_TYPE_H

/*
 * it's set when we have not asked for any service and the server contacts us
*/
#define PRIVATE_NONE_REQUEST 5

namespace n_request {
    enum e_request{
        login_user = 0,
        register_user = 1,

        download_file = 2,
        save_file = 3,

        balance = 4,
        none = PRIVATE_NONE_REQUEST
    };
}

#endif // REQUEST_TYPE_H
