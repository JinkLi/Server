//
// Created by Josin on 2018/6/28.
//

#ifndef SERVER_HTTP_SERVER_HPP
#define SERVER_HTTP_SERVER_HPP

#include "http_parser.hpp"
#include "../tcp_server.hpp"
#include "../include/common.h"

typedef struct __server_data
{
    w_string *data;
    w_len length;
    w_u_char p_status;
} _server_data;

class http_server : public tcp_server<char>
{
    private:
        __server_data *t_data;
        w_len content_length;

    public:
        /**
         * Constructor of the http_server
         * @param host
         * @param port
         * @param backlog
         */
        http_server( w_string host, w_u_int port, w_u_int backlog);

        /**
         * Read 8192 bytes data for each recv invoke
         * @param client_fd  The client_fd which you want to recv data from
         * @return w_string, if you want the c string, call 'c_str()'
         */
        _server_data *recv_data(int client_fd );

        /**
         * Send data to the client which corresponding to the client_fd
         * @param client_fd
         * @param data
         * @return
         */
        w_len send_data(int client_fd, w_string data) ;

        /**
         * Finalize function
         */
        ~http_server() override;
};

http_server::http_server(w_string host, w_u_int port, w_u_int backlog): tcp_server(host, port, backlog)
{
    this->t_data = new _server_data;
    this->t_data->data = new w_string;
    this->t_data->length = 0;
    this->t_data->p_status = 0;
    this->content_length = 0;
}

http_server::~http_server()
{
    delete this->t_data;
}

_server_data *http_server::recv_data(int client_fd)
{
    http_parser httpParser;
    w_u_char start = { 0 };
    this->content_length = 0;
    char buff[ 8192 ] = { 0 };
    w_len length, left_len = {0}, body_length = {0};
    this->t_data->p_status = 1;
    this->t_data->data->erase(this->t_data->data->begin(), this->t_data->data->end());

    while ( 1 )
    {
        bzero(buff, sizeof(buff));
        length  = tcp_server::recv_data(
                     client_fd,
                     buff,
                     (left_len == 0) ? sizeof(buff) : ( (left_len > sizeof(buff)) ? sizeof(buff) : left_len )
        );
        if ( length <= 0 ) {
            this->t_data->p_status = length;
            break;
        }

        this->t_data->data->append(buff);

        if ( length <= sizeof(buff) )
        {
            if ( !this->content_length )
            {
                w_map *http_data = httpParser.get_result(this->t_data->data);

                if((*http_data)["Content-Length"].size() == 0 && (*http_data)["content-length"].size() == 0) break;
                left_len = this->content_length = std::stoi(
                        (*http_data)["Content-Length"].size() ? (*http_data)["Content-Length"] : (*http_data)["content-length"]
                );
                body_length = (*http_data)["HTTP-BODY"].size();
            }
            else
            {
                body_length = length;
            }

            this->t_data->length += body_length;

            if (left_len < sizeof(buff) || body_length == left_len)
            {
                break;
            }
            else
            {
                left_len = left_len - body_length;
            }
        }
    }
    return this->t_data;
}

w_len http_server::send_data(int client_fd, w_string data)
{
    return send(client_fd, data.c_str(), data.size(), 0);
}

#endif //SERVER_HTTP_SERVER_HPP
