//
// Created by Josin on 2018/6/26.
//

#ifndef SERVER_TCP_SERVER_H
#define SERVER_TCP_SERVER_H

#include "include/common.h"

template <typename T>
class tcp_server
{
    private:
        T *t_data;

    protected:
        int socket_fd;

    public:
        tcp_server(w_string host, w_u_int port, w_u_int backlog);

        int get_client(sock_addr_in *client_addr);

        w_len recv_data( int client_fd, T *data, w_len data_len );

        T *recv_all_data( int client_fd, w_len *data_count );

        virtual ~tcp_server();
};

template<typename T>
tcp_server<T>::tcp_server(w_string host, w_u_int port, w_u_int backlog) : t_data(nullptr)
{
    int ret_val;
    sock_addr_in sockaddr;

    this->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (this->socket_fd == -1)
    {
        perror("Create socket failed.");
    }

    bzero(&sockaddr, sizeof(sockaddr));
    inet_pton(AF_INET, host.c_str(), &sockaddr.sin_addr);
    sockaddr.sin_port = htons(port);
    sockaddr.sin_family = AF_INET;
    ret_val = bind(this->socket_fd, (sock_addr *)&sockaddr, sizeof(sockaddr));
    if (ret_val == -1)
    {
        perror("Bind socket failed.");
    }

    ret_val = listen(this->socket_fd, backlog + 1);
    if (ret_val == -1)
    {
        perror("Listen socket failed.");
    }
}

template<typename T>
tcp_server<T>::~tcp_server()
{
    free(this->t_data);
}

template<typename T>
int tcp_server<T>::get_client(sock_addr_in *client_addr)
{
    w_u_int len = sizeof(sock_addr_in);
    return accept(this->socket_fd, (sock_addr *)client_addr, &len);
}

template<typename T>
w_len tcp_server<T>::recv_data( int client_fd, T *data, w_len data_len )
{
    return recv(client_fd, (void *)data, data_len, 0);
}

template<typename T>
T *tcp_server<T>::recv_all_data(int client_fd, w_len *data_count)
{
    *data_count = 0;
    free(this->t_data);
    this->t_data = (T *)malloc(sizeof(T));
    w_len num = 2;
    w_len len = recv(client_fd, this->t_data, sizeof(T), 0);
    ++(*data_count);

    while (len)
    {
        this->t_data = (T *)realloc(this->t_data, sizeof(T) * num);
        len = recv(client_fd, this->t_data + num - 1, sizeof(T), 0);
        ++(*data_count);
        ++num;
    }

    --(*data_count);
    return this->t_data;
}

#endif //SERVER_TCP_SERVER_H
