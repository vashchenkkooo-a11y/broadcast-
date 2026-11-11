#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
class client
{
public:
    client(boost::asio::io_context &io_context);
    void write(const std::string& request);
    void connect(tcp::resolver::results_type& endpoints);

private:
    tcp::socket socket_;
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
    void read();
    void handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred);
     std::array<char, 512> buf_;
};
