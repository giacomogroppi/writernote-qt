#include "cloud_controll.h"
#include "utils/remove_key/remove_key.h"
#include "utils/setting_define.h"

#ifdef CLOUD

cloud_controll::cloud_controll(struct struct_user *user)
{
    m_user = user;

    /*
     * when the memory of this instance is freed also m_socket will be freed
     * thanks to qt
    */
    m_socket = new QTcpSocket(this);

    WObject::connect(m_socket, &QTcpSocket::errorOccurred, this, &cloud_controll::error_socket);
    WObject::connect(m_socket, &QTcpSocket::readyRead, this, &cloud_controll::ready_read);
    WObject::connect(m_socket, &QTcpSocket::disconnected, this, &cloud_controll::disconnected);

}

n_error_cloud::e_error_cloud cloud_controll::action(n_request::e_request m_request, struct_user *user)
{
    if(this->m_request)
        return n_error_cloud::wait;

    if (user == NULL and m_user == NULL)
        return n_error_cloud::not_login;

    if(user != NULL){
        save_recent_user(user);
        m_user = user;
    }

    QByteArray array;

    this->m_last_request = m_request;

    if(!m_socket->isWritable()){
        if(this->connect_socket() != n_error_socket::ok_socket){
            emit error_connection();
            return n_error_cloud::server_down;
        }
    }


    if(m_request == n_request::login_user){
        if(user == nullptr)
            return n_error_cloud::not_login;

        int temp = m_request;
        array.append((const char *)&temp, sizeof(int));

        array.append((const char *)user, sizeof(struct struct_user));

        m_socket->write(array);

        /*
         * if it takes more than TIME_WRITE ms to send data
        */
        if(!m_socket->waitForBytesWritten(TIME_WRITE))
            return n_error_cloud::server_down;

        this->m_request = true;

    }else if(m_request == n_request::balance){
        if(user == nullptr)
            return n_error_cloud::not_login;

    }

    return n_error_cloud::ok;
}

/*
 * send data to the server to make a new user
*/
n_error_cloud::e_error_cloud cloud_controll::registerUser(struct struct_user *user)
{
    if(user == nullptr)
        return n_error_cloud::internal_error;

    QByteArray array;
    int temp = n_request::register_user;

    array.append((const char *) &temp, sizeof(int));

    array.append((const char *)user, sizeof(struct struct_user));

    this->m_socket->write(array);

    if(!this->m_socket->waitForBytesWritten(TIME_WRITE))
        return n_error_cloud::server_down;

    this->m_last_request = n_request::register_user;
    this->m_request = true;

    return n_error_cloud::ok;
}

bool cloud_controll::write(QByteArray &array, n_request::e_request r){
    this->m_socket->write(array);

    if(!this->m_socket->waitForBytesWritten(TIME_WRITE))
        return false;

    this->m_last_request = r;
    this->m_request = true;

    return true;
}

n_error_cloud::e_error_cloud cloud_controll::loginUser(struct struct_user *user){
    if(user == nullptr)
        return n_error_cloud::internal_error;

    int temp = n_request::login_user;
    QByteArray array;
    array.append((const char *)&temp, sizeof(int));

    array.append((const char *)user, sizeof(struct struct_user));

    if(!this->write(array, n_request::login_user))
        return n_error_cloud::server_down;

    return n_error_cloud::ok;
}

void cloud_controll::cleanUser()
{
    if(this->m_user)
        delete this->m_user;

    remove_key(GROUPNAME_CLOUD_USER, KEY_USER_CLOUD_IS_DEFINED);
    remove_key(GROUPNAME_CLOUD_USER, KEY_USER_CLOUD_STRUCT);

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
void cloud_controll::error_socket(QAbstractSocket::SocketError)
{

}

void cloud_controll::ready_read(){
    emit this->readyReadExt(m_socket->readAll(), this->m_last_request);
    this->m_request = false;
    this->m_last_request = n_request::none;
}

void cloud_controll::disconnected(){

}

#endif // CLOUD
