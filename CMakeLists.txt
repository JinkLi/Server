cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)

add_executable(server main.cpp kernel/tcp_server.hpp kernel/udp_server.hpp kernel/include/common.h kernel/tcp_client.hpp kernel/http/http_server.hpp kernel/http/http_client.hpp kernel/http/http_parser.hpp)