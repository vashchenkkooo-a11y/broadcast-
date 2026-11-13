#include "serializer.hpp"
#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../shared/const.hpp"

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

std::string serializer::make_send_request(std::string nickname, std::string message) {
        boost::json::object obj;
    boost::json::object data;

    obj["event"] = Event::send;
    data["nickname"] = nickname;
    data["message"] = message;
    obj["data"] = data;
    std::string s  = boost::json::serialize(obj);
    return s;
}

std::string serializer::make_logout_request() {
    boost::json::object obj;
    obj["event"] = Event::logout;
    obj["data"] = boost::json::object{};
    std::string s = boost::json::serialize(obj);
    return s;
}