#pragma once
#include <boost/json.hpp>
#include "client.hpp"
#include "../shared/parser.hpp"
#include "serializer.hpp"

class auth_service {
public:
    auth_service(client &client);
    void start();
private:
    client &client_;
    serializer serializer_; 
    void handle_menu();
};