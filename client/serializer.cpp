#include "serializer.hpp"
#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/json.hpp>

using boost::asio::ip::tcp;

std::string serializer::make_regauth_request(std::string login, std::string password, std::string event_name) {
    boost::json::object obj;
    boost::json::object obj1;

    obj1["event"] = event_name;
    obj["login"] = login;
    obj["password"] = password;
    obj1["data"] = obj; 
    std::string s = boost::json::serialize(obj1); //json text
    return s;
}
