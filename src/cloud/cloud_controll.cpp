#include "cloud_controll.h"

cloud_controll::cloud_controll(struct struct_user *user)
{
    m_user = user;

    /*
     * when the memory of this instance is freed also m_socket will be freed
     * thanks to qt
    */
    m_socket = new QTcpSocket(this);

    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                this, &cloud_controll::error_socket);

    connect(m_socket, &QTcpSocket::readyRead, this, &cloud_controll::ready_read);
    connect(m_socket, &QTcpSocket::disconnected, this, &cloud_controll::disconnected);

}

n_error_cloud::e_error_cloud cloud_controll::action(n_request::e_request m_request, void **pointer, struct_user *user)
{
    if (user == nullptr && m_user == NULL){
        return n_error_cloud::not_login;
    }

    if(m_request == n_request::e_request::login_user){
        if(!m_socket->isWritable()){
            if(this->connect_socket() != n_error_socket::ok_socket)
                emit error_connection();
        }
    }

    return n_error_cloud::ok;
}

n_error_socket::e_error_socket cloud_controll::connect_socket()
{
    if(m_socket->isWritable())
        return n_error_socket::ok_socket;

    m_socket->connectToHost(QString::fromUtf8(URL_SERVER), PORT);

    return n_error_socket::ok_socket;
}

/*
 * error in the socket
*/
void cloud_controll::error_socket(QAbstractSocket::SocketError){

}

void cloud_controll::ready_read(){

}

void cloud_controll::disconnected(){

}
