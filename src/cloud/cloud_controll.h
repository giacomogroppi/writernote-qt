#ifndef CLOUD_CONTROLL_H
#define CLOUD_CONTROLL_H

#include "struct_user.h"
#include "request_type.h"
#include "error_type.h"

#include <QTcpSocket>
#include <QObject>

#include "url.h"

/*
 * time waiting for responde. [int ms]
*/
#define TIME_RESPONE 3000

namespace n_error_socket {
    enum e_error_socket{
        ok_socket,
        error_
    };
}


class cloud_controll: public QObject
{
    Q_OBJECT
public:
    cloud_controll(struct struct_user *user = NULL);

    /*by default it will be use m_user [from the instance of the class]*/
    n_error_cloud::e_error_cloud action(n_request::e_request, void **pointer, struct struct_user *user = NULL);

    struct struct_user *m_user; /* in case NULL is set by cloud_controll::cloud_controll */

private:
    /*
     * if the m_request is set to true
     * it means that the system is
     * waiting for a response from the server,
     * otherwise it is set to false
    */
    bool m_request = false;

    QTcpSocket *m_socket = NULL;

    /*function is call to connect the socket to the server*/
    n_error_socket::e_error_socket connect_socket();

signals:
    void readyReadExt(QByteArray &);

public slots:
    void error_socket(QAbstractSocket::SocketError);
    void ready_read();
    void disconnected();

signals:
    void error_connection();
};

#endif // CLOUD_CONTROLL_H
