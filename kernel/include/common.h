//
// Created by Josin on 2018/6/26.
//

#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <cctype>
#include <string>
#include <fcntl.h>
#include <map>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

typedef bool          w_bool;
typedef unsigned int  w_u_int;
typedef unsigned char w_u_char;
typedef unsigned long w_u_long;
typedef ssize_t       w_len;
typedef std::string   w_string;
typedef struct sockaddr_in sock_addr_in;
typedef struct sockaddr    sock_addr;
typedef std::map<w_string, w_string> w_map;
typedef std::pair<w_string, w_string> w_pair;

#endif //SERVER_COMMON_H
