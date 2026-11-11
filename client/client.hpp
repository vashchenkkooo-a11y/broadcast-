#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
class client
{
public:
    client(boost::asio::io_context &io_context);
    std::string read();
    void write(const std::string& request);
    void connect(tcp::resolver::results_type& endpoints);

private:
    tcp::socket socket;
};
