//
// Created by Josin on 2018/6/28.
//

#ifndef SERVER_HTTP_CLIENT_HPP
#define SERVER_HTTP_CLIENT_HPP

#include "../tcp_client.hpp"
#include "../include/common.h"

class http_client : public tcp_client<w_string>
{

    public:
        http_client(w_string host, w_u_int port);

        w_len send_data(char *data, w_len data_len);
};

http_client::http_client(w_string host, w_u_int port) : tcp_client(host, port)
{

}

w_len http_client::send_data(char *data, w_len data_len)
{
    return send(this->socket_fd, data, data_len, 0);
}

#endif //SERVER_HTTP_CLIENT_HPP
