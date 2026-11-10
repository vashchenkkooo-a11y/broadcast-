#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
class client
{
public:
    client(boost::asio::io_context &io_context, tcp::resolver::results_type &endpoints);
    std::string read();
    void write(const std::string& request);

private:
    tcp::socket socket;
};
