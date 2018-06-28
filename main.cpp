#include <iostream>

#include "kernel/tcp_client.hpp"
#include "kernel/tcp_server.hpp"
#include "kernel/http/http_data.hpp"
#include "kernel/http/http_client.hpp"
#include "kernel/http/http_server.hpp"

int main(int argc, char *argv[])
{
    sock_addr_in client_addr;
    http_server httpServer("127.0.0.1", 8000, 5);
    int c_id = httpServer.get_client(&client_addr);


    http_data httpData;
    httpData.set_status_code(200);
    httpData.set_response_header("Content-Type", "application/json;charset=UTF-8");
    // httpData.set_response_header("Connection", "close");
    httpData.set_response_body("{\"Engine\":\"CSpeed\", \"Version\":\"v2.1.13\", \"BenchMark\":\"10234req/sec\"}");

    while (1)
    {
        _server_data *data = httpServer.recv_data(c_id);
        if (data->p_status <= 0)
        {
            break;
        }
        std::cout << httpServer.send_data(c_id, *(httpData.get_response_text())) << std::endl;
        std::cout << "[" << data->data->c_str() << "]" << std::endl;
    }

    // w_string data;
    // data.append("GET / HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: 68\r\n\r\n");
    // data.append("{\"Engine\":\"CSpeed\", \"Version\":\"v2.1.13\", \"BenchMark\":\"10234req/sec\"}");
    // http_client httpClient("127.0.0.1", 8000);
    // httpClient.send_data((char *)data.c_str(), data.size());



    return 0;
}