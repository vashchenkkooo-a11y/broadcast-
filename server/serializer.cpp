#include "serializer.hpp"
#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/json.hpp>

using boost::asio::ip::tcp;

std::string serializer::make_response(std::string status, std::string message) {
    boost::json::object obj;

    obj["status"] = status;
    obj["message"] = message;

    std::string s = boost::json::serialize(obj); //json text
    return s;
}
