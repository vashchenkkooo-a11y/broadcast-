#include "client.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
//только сеть

using boost::asio::ip::tcp;
client::client(boost::asio::io_context &io_context) 
    : socket_(io_context)
{
    // boost::asio::connect(socket, endpoints);
}

void client::connect(tcp::resolver::results_type& endpoints) {
    boost::asio::connect(socket_, endpoints);
}


    
void client::write(const std::string &request) {
    boost::system::error_code error;
    boost::asio::async_write(socket_, boost::asio::buffer(request),
                           boost::bind(&client::handle_write, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
    
                           }
void client::handle_write(const boost::system::error_code& error, size_t bytes_transferred) 
{   
    if (error) throw boost::system::system_error(error);
    read();
}

std::string client::read() {
    socket_.async_read_some(boost::asio::buffer(buf_),
                    boost::bind(&client::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    return std::string(buf_.data());
}

void client::handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred) {
    if (error) throw boost::system::system_error(error);
}