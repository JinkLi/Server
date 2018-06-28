##C++开发的服务器socket通信库##

##原因##

练习socket套接字编程，强化中间的通信环节，以及TCP自动分包(粘包)、合包的处理手法

##现状##

完成了基础的 **HTTP** 服务针对于 HTTP 请求，每次请求服务端都会 POST 请求传送的 Content-Length 字段标识的长度进行数据包的读取以及合并数据包

长连接请使用原始的 **tcp_server**，因为长连接的数据是一个 **tcp** 数据流，没有数据边界，需要人工处理，假设如下：

**A**客户机 发送了 **8kb** 数据至 **B**服务器，由于是长连接，故数据流与数据流之间的边界需要依靠其他的手段进行处理：如：流报文长度标识或者数据流的结束标识，现在在 **8kb** 数据中：假设只有 1020字节是第一个请求的，那么在后续的8192-1020字节的数据则需要放入缓存队列，等待下次系统的数据读取操作的时候返回，以及或许还要进行数据的截取操作等，很是麻烦。

##操作demo##

**服务端代码**

```c
sock_addr_in client_addr;
http_server httpServer("127.0.0.1", 8000, 5);
int c_id = httpServer.get_client(&client_addr);


http_data httpData;
httpData.set_status_code(200);
httpData.set_response_header("Content-Type", "application/json;charset=UTF-8");
httpData.set_response_body("{\"Engine\":\"CSpeed\", \"Version\":\"v2.1.13\", \"BenchMark\":\"10234req/sec\"}");

while (1)
{
    _server_data *data = httpServer.recv_data(c_id);
    if (data->p_status <= 0)
    {
        break;
    }
    httpServer.send_data(c_id, *(httpData.get_response_text()));
    std::cout << "[" << data->data->c_str() << "]" << std::endl;
}

```

**客户端代码**

```c
w_string data;
data.append("GET / HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: 68\r\n\r\n");
data.append("{\"Engine\":\"CSpeed\", \"Version\":\"v2.1.13\", \"BenchMark\":\"10234req/sec\"}");
http_client httpClient("127.0.0.1", 8000);
httpClient.send_data((char *)data.c_str(), data.size());
```