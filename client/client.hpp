#pragma once
#include <boost/asio.hpp>
#include "serializer.hpp"
#include "../shared/const.hpp"
#include <boost/json.hpp>



using boost::asio::ip::tcp;
class client
{
public:
    client(boost::asio::io_context &io_context, tcp::resolver::results_type &endpoints);
    boost::json::object regauth(std::string &login, std::string &password, const std::string& event);

private:
    tcp::socket socket;
};
