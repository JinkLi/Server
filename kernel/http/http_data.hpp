//
// Created by Josin on 2018/6/26.
//

#ifndef WEBSERVER_HTTP_DATA_H
#define WEBSERVER_HTTP_DATA_H

#include "../include/common.h"

// Some status code for http status
int codes[] = {
    100, 101, 102, 118,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 210, 226,
    300, 301, 302, 303, 304, 305, 306, 307, 308, 310,
    400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 421, 422, 423,
    424, 425, 426, 428, 429, 431, 449, 450, 451,
    500, 501, 502, 503, 504, 505, 507, 508, 509, 510, 511
};
w_string status_text[] = {
    "Continue", "Switching Protocols", "Processing", "Connection timed out",
    "OK", "Created", "Accepted", "Non-Authoritative", "No Content", "Reset Content", "Partial Content", "Multi-Status",
    "Already Reported", "Content Different", "IM Used",
    "Multiple Choices", "Moved Permanently", "Found",
    "See Other", "Not Modified", "Use Proxy", "Reserved", "Temporary Redirect", "Permanent Redirect",
    "Too many Redirect", "Bad Request", "Unauthorized", "Payment Required", "Forbidden", "Not Found",
    "Method Not Allowed", "Not Acceptable", "Proxy Authentication Required", "Request Time-out", "Conflict",
    "Gone", "Length Required", "Precondition Failed", "Request Entity Too Large", "Request-URI Too Long", "Unsupported Media Type",
    "Requested range unsatisfiable", "Expectation failed", "I'm a teapot", "Misdirected Request", "Unprocessable entity",
    "Locked", "Method failure",
    "Unordered Collection", "Upgrade Required", "Precondition Required", "Too Many Requests", "Request Header Fields Too Large",
    "Retry With", "Blocked by Windows Parental Controls", "Unavailable For Legal Reasons",
    "Internal Server Error", "Not Implemented", "Bad Gateway or Proxy Error", "Service Unavailable", "Gateway Time-out",
    "HTTP Version not supported", "Insufficient storage", "Loop Detected", "Bandwidth Limit Exceeded", "Not Extended",
    "Network Authentication Required"
};

class http_data
{
    private:
        w_map *response_headers;

        w_map *http_codes;

        w_u_int status_code;

        w_string status;

        w_string request_method;

        w_u_int len;

        w_string *body;

        w_string *response_text;

    public:
        /**
         * Constructor of the http_data class
         */
        http_data();

        /**
         * D-constructor
         */
        ~http_data();

        /**
         * Set the status code with the status Text
         * if the code not exists in the codes, 200 will instead of it.
         * @param code 200 or 100 or 301 and so on.
         * @return true/false
         */
        w_bool set_status_code(w_u_int code);

        /**
         * Set the response header with the key and vlaue
         * @param key    response key
         * @param value  response value
         * @return       true/false
         */
        w_bool set_response_header(w_string key, w_string value);

        /**
         * Set the response body with the given body
         * @param body The body which will be send to client
         * @return     true/false
         */
        w_bool set_response_body(w_string body);

        /**
         * Get the fully text which for the http response
         * @return w_string *
         */
        w_string *get_response_text();

        /**
         * Set the request method
         * @param method GET or POST and so on.
         * @return
         */
        w_bool set_request_method(w_string method);
};

http_data::http_data()
{
    this->response_headers = new w_map;
    this->http_codes       = new w_map;
    this->body             = new w_string;
    this->response_text    = new w_string;

    this->len = sizeof(codes)/sizeof(int);

    for ( int i = 0; i < sizeof(codes)/sizeof(int) ; ++i)
    {
        this->http_codes->insert(std::pair<w_string, w_string>( std::to_string(codes[i]), status_text[i] ) );
    }
}

http_data::~http_data()
{
    delete this->body;
    delete this->http_codes;
    delete this->response_headers;
    delete this->response_text;
}

w_bool http_data::set_status_code(w_u_int code)
{
    w_map *codes = this->http_codes;
    w_string status = (*codes)[std::to_string(code)];
    if ( 0 == status.size() )
    {
        this->status_code = 200;
        this->status = "OK";
    }
    else
    {
        this->status_code = code;
        this->status      = status;
    }
    return true;
}

w_bool http_data::set_response_header(w_string key, w_string value)
{
    std::pair<w_map::iterator, bool> ret = this->response_headers->insert(std::pair<w_string, w_string>( key, value ));
    return ret.second;
}

w_bool http_data::set_response_body(w_string body)
{
    this->body->assign( body );
    std::pair<w_map::iterator, bool> ret = this->response_headers->insert(
    std::pair<w_string, w_string>( "content-length", std::to_string(body.size()) ));
    return true;
}

w_string *http_data::get_response_text()
{
    if (this->response_text->size() > 0)
    {
        return this->response_text;
    }
    this->response_text->append("HTTP/1.1 ");
    this->response_text->append(std::to_string(this->status_code));
    this->response_text->append(" ");
    this->response_text->append(this->status);
    this->response_text->append(" \r\n");
    for ( auto head : *this->response_headers )
    {
        this->response_text->append(head.first);
        this->response_text->append(": ");
        this->response_text->append(head.second);
        this->response_text->append("\r\n");
    }
    this->response_text->append("\r\n");
    this->response_text->append(*(this->body));

    return this->response_text;
}

w_bool http_data::set_request_method(w_string method)
{
    this->request_method = method;
    return true;
}

#endif //WEBSERVER_HTTP_DATA_H
