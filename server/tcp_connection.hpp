#pragma once
#include <string>
#include <map>
#include <boost/asio.hpp> 
#include <boost/json.hpp>

using boost::asio::ip::tcp;

class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  typedef std::shared_ptr<tcp_connection> pointer;
  static pointer create(boost::asio::io_context& io_context, std::map<std::string, std::string> &data_, std::map<std::string, tcp_connection::pointer> &active_connections_);
  tcp::socket& socket();
  void start();
  void write(const std::string& response);

private:
  tcp_connection(boost::asio::io_context& io_context, std::map<std::string, std::string> &data_, std::map<std::string, tcp_connection::pointer> &active_connections_);
  void read();
  void handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred);
  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/);
  
  
  void handle_reg(boost::json::object& data);
  void handle_auth(boost::json::object& data);

  void handle_send(boost::json::object& data);

  tcp::socket socket_;
  std::string message_;
  std::array<char, 512> buf_;
  std::map<std::string, std::string> &data_;
  std::string response_; 
  std::string login_;
  std::map<std::string, tcp_connection::pointer> &active_connections_;

};
