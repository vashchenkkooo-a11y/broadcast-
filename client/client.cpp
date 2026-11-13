#include "client.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include "../shared/parser.hpp"


using boost::asio::ip::tcp;
client::client(boost::asio::io_context &io_context) 
    : socket_(io_context)
{
    // boost::asio::connect(socket, endpoints);
}

void client::connect(tcp::resolver::results_type& endpoints) { //асинх
    boost::asio::connect(socket_, endpoints);
}


    
void client::write(const std::string &request) {
    boost::system::error_code error;
    auto buf = std::make_shared<std::string>(request);
    boost::asio::async_write(socket_, boost::asio::buffer(*buf),
                           boost::bind(&client::handle_write, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
    
                           }
void client::handle_write(const boost::system::error_code& error, size_t bytes_transferred) 
{   
    if (error) throw boost::system::system_error(error);
    read();
}

void client::read() {
    socket_.async_read_some(boost::asio::buffer(buf_),
                    boost::bind(&client::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

}

void client::handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred) {
    if (error) throw boost::system::system_error(error);

    parser parser_;
    boost::json::object obj = parser_.parse(std::string(buf_.data(), bytes_transferred));
    std::cout << obj["message"].as_string().c_str() << std::endl;
    
    read();
}