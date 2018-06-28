//
// Created by Josin on 2018/6/28.
//

#ifndef SERVER_HTTP_PARSER_HPP
#define SERVER_HTTP_PARSER_HPP

#include "../include/common.h"

class http_parser
{
    private:
        w_map *header;

    public:
        http_parser();

        ~http_parser();

        w_map *get_result(w_string *buff);


};

http_parser::http_parser()
{
    this->header = new w_map;
}

http_parser::~http_parser()
{
    delete this->header;
}

w_map *http_parser::get_result(w_string *buff)
{
    w_bool method_get = false;
    this->header->erase(this->header->begin(), this->header->end());

    w_string::size_type curr_pos = 0, space_pos, colon_pos = 0, str_count = {0};

    for (int i = 0; i < buff->size(); ++i)
    {
        if ( buff->at(i) == ' ' && str_count < 2 )
        {
            space_pos = i;
            str_count++;
            if (str_count == 1)
            {
                if ( !buff->substr(curr_pos, space_pos - curr_pos).compare("GET") )
                {
                    method_get = true;
                }
                this->header->insert(w_pair("REQUEST-METHOD", buff->substr(curr_pos, space_pos - curr_pos)));
            }
            else if (str_count == 2)
            {
                this->header->insert(w_pair("REQUEST-URL", buff->substr(curr_pos, space_pos - curr_pos)));
            }
            curr_pos = i + 1;
            continue;
        }

        if (buff->at(i) == ':' && buff->at(i + 1) == ' ')
        {
            colon_pos = i;
        }

        if (buff->at(i) == '\r' && buff->at(i + 1) == '\n')
        {
            if (str_count < 2)
            {
                return nullptr;
            }
            else if (str_count == 2)
            {
                this->header->insert(w_pair("HTTP-VERSION", buff->substr(curr_pos, i - curr_pos)));
                str_count++;
                curr_pos = i + 2;
            }
            else if (str_count > 2)
            {
                if (buff->at(i + 2) == '\r' && buff->at(i + 3) == '\n')
                {
                    this->header->insert(w_pair(buff->substr(curr_pos, colon_pos - curr_pos), buff->substr(colon_pos + 2, i - colon_pos - 2)));
                    this->header->insert(w_pair("HTTP-BODY", buff->substr(i + 4, buff->size())));
                    if (method_get)
                    {
                        this->header->insert(w_pair("Content-Length", std::to_string(buff->substr(i + 4, buff->size()).size())));
                    }
                    break;
                }
                else if (buff->at(i + 2) != '\r' && buff->at(i + 3) != '\n')
                {
                    if (colon_pos == 0)
                    {
                        return nullptr;
                    }
                    this->header->insert(w_pair(buff->substr(curr_pos, colon_pos - curr_pos), buff->substr(colon_pos + 2, i - colon_pos - 2)));
                    curr_pos = i + 2;
                }
            }
        }
    }
    return this->header;
}

#endif //SERVER_HTTP_PARSER_HPP
