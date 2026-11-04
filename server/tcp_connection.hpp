#pragma once
#include <string>
#include <boost/asio.hpp> 

using boost::asio::ip::tcp;
class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  typedef std::shared_ptr<tcp_connection> pointer;
  static pointer create(boost::asio::io_context& io_context);
  tcp::socket& socket();
  void start();
  void write(boost::asio::const_buffer b);

private:
  tcp_connection(boost::asio::io_context& io_context);
  void read(boost::asio::mutable_buffer b);
  void handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred);
  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/);
      
  tcp::socket socket_;
  std::string message_;
  std::array<char, 512> buf_;
};
