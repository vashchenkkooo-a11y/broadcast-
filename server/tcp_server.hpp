#pragma once
#include <boost/asio.hpp> 
#include "tcp_connection.hpp"
#include <vector>

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context);

private:
  void start_accept();

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error);

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
  std::vector<std::string> logins_;
  std::map<std::string, tcp_connection::pointer> active_connections_;
  std::map<std::string, std::string> data_; //заменить на деф, тут логин и пароль
};

