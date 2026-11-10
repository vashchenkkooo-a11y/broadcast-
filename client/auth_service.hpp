#pragma once
#include <boost/json.hpp>
#include "client.hpp"
#include "../shared/parser.hpp"

class auth_service {
public:
    boost::json::object regauth(std::string &login, std::string &password, const std::string &event);
    auth_service(client &client);
    private:
    client &client_; 
};