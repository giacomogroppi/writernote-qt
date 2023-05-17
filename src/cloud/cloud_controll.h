#pragma once

#include "struct_user.h"
#include "request_type.h"
#include "error_type.h"
#include "Scheduler/WObject.h"

#include "url.h"
#ifdef CLOUD

#include <QTcpSocket>

/*
 * time waiting for responde. [int ms]
*/
#define TIME_WRITE 3000

namespace n_error_socket {
    enum e_error_socket{
        ok_socket,
        error_
    };
}


class cloud_controll: public WObject
{
    Q_OBJECT
public:
    cloud_controll(struct struct_user *user = NULL);

    /*by default it will be use m_user [from the instance of the class]*/
    n_error_cloud::e_error_cloud action(n_request::e_request, struct struct_user *user = NULL);

    struct struct_user *m_user; /* in case NULL is set by cloud_controll::cloud_controll */

    n_error_cloud::e_error_cloud registerUser(struct_user *user);
    n_error_cloud::e_error_cloud loginUser(struct_user *user);

    /*
     * remove the current user if exist
    */
    void cleanUser();

private:
    /*
     * return true if all is written in the socket correctly
    */
    bool write(WByteArray &,  n_request::e_request);

    /*
     * in m_last_request is store the last
     * request we had made
    */
    bool m_request = false;
    n_request::e_request m_last_request = n_request::none;

    QTcpSocket *m_socket = NULL;

    /*function is call to connect the socket to the server*/
    n_error_socket::e_error_socket connect_socket();

signals:
    void readyReadExt(WByteArray, n_request::e_request);

public slots:
    void error_socket(QAbstractSocket::SocketError);
    void ready_read();
    void disconnected();

signals:
    void error_connection();
};

#else
class cloud_controll {};

#endif // CLOUD
