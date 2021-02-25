#ifndef CLOUD_CONTROLL_H
#define CLOUD_CONTROLL_H

#include "struct_user.h"
#include "request_type.h"
#include "error_type.h"

#include <QTcpSocket>
#include <QObject>

#include "url.h"

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
    n_error_cloud::e_error_cloud action(n_request::e_request, struct struct_user *user = NULL);

private:
    QTcpSocket *m_socket = NULL;
    struct struct_user *m_user;

    /*function is call to connect the socket to the server*/
    n_error_socket::e_error_socket connect_socket();


public slots:
    void error_socket(QAbstractSocket::SocketError);
    void ready_read();
    void disconnected();

signals:
    void error_connection();
};

#endif // CLOUD_CONTROLL_H
