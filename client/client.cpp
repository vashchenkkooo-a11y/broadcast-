#include "client.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include "../shared/parser.hpp"

using boost::asio::ip::tcp;
client::client(boost::asio::io_context &io_context)
    : io_context_(io_context), socket_(io_context), reconnect_timer_(io_context)
{
    // boost::asio::connect(socket, endpoints);
}

void client::connect(tcp::resolver::results_type &endpoints)
{
    endpoints_ = endpoints;
    boost::asio::async_connect(socket_, endpoints_,
                               boost::bind(&client::handle_connect, this,
                                           boost::asio::placeholders::error));
}

void client::handle_connect(const boost::system::error_code &error)
{
    if (error)
    {
        std::cerr << "Connect error: " << error.message() << std::endl;
        reconnect();
        return;
    }
    std::cout << "Connected!" << std::endl; 
    read();
}

void client::reconnect()
{
    std::cout << "Reconnecting in 5s" << std::endl;
    boost::system::error_code ec;
    socket_.close(ec);

    reconnect_timer_.expires_after(std::chrono::seconds(5));
    reconnect_timer_.async_wait(
        boost::bind(&client::handle_reconnect, this,
                    boost::asio::placeholders::error));
}

void client::handle_reconnect(const boost::system::error_code &ec)
{
    if (!ec)
    {
        socket_ = tcp::socket(io_context_);
        connect(endpoints_);
        
    }
}

void client::write(const std::string &request)
{
    boost::system::error_code error;
    boost::asio::async_write(socket_, boost::asio::buffer(request),
                             boost::bind(&client::handle_write, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
void client::handle_write(const boost::system::error_code &error, size_t bytes_transferred)
{
    if (error)
    {
        std::cerr << "Connect error: " << error.message() << std::endl;
        reconnect();
        return;
    }
    read();
}

void client::read()
{
    socket_.async_read_some(boost::asio::buffer(buf_),
                            boost::bind(&client::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void client::handle_read(const boost::system::error_code &error,
                         std::size_t bytes_transferred)
{
    if (error)
    {
        std::cerr << "Read error: " << error.message() << std::endl;
        reconnect();
        return;
    }
    parser parser_;
    boost::json::object obj = parser_.parse(std::string(buf_.data(), bytes_transferred));
    std::cout << obj["message"].as_string().c_str() << std::endl;

    read();
}