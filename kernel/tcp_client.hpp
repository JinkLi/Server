//
// Created by Josin on 2018/6/26.
//

#ifndef SERVER_TCP_CLIENT_HPP
#define SERVER_TCP_CLIENT_HPP

#include "include/common.h"

template <typename T>
class tcp_client
{
    protected:
        int socket_fd;

    public:
        tcp_client(w_string host, w_u_int port);

        w_len send_data(T *data, w_len data_len);
};

template<typename T>
tcp_client<T>::tcp_client(w_string host, w_u_int port)
{
    int ret_val;
    sock_addr_in server_addr;

    this->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (this->socket_fd == -1)
    {
        perror("Create socket failed: ");
    }

    bzero(&server_addr, sizeof(server_addr));
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    ret_val = connect(this->socket_fd, (sock_addr *)&server_addr, sizeof(server_addr));
    if (ret_val == -1)
    {
        perror("Connect server failed: ");
    }
}

template<typename T>
w_len tcp_client<T>::send_data(T *data, w_len data_len)
{
    return send(this->socket_fd, data, data_len, 0);
}

#endif //SERVER_TCP_CLIENT_HPP
