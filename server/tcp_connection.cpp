#include "tcp_connection.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>

enum Error {
    LOGIN_EXISTS,
    INVALID_CREDENTIALS,
    RECIPIENT_NOT_FOUND,
    NOT_LOGGED_IN,
    HISTORY_NOT_FOUND 
};

void tcp_connection::start() {
  read(boost::asio::buffer(buf_));
}

tcp_connection::pointer tcp_connection::create(boost::asio::io_context& io_context) {
  return pointer(new tcp_connection(io_context)); 
}
tcp::socket &tcp_connection::socket() {
   return socket_;
}

   void tcp_connection::write(boost::asio::const_buffer b) {
      socket_.async_write_some(b,
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }

  tcp_connection::tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context) //cюда вектор надо добавить (чтоб тсп коннекнш кроме сокета и ио контекста имел еще ссылку на вектор)
  {}

  void tcp_connection::read(boost::asio::mutable_buffer b) {
        socket_.async_read_some(b,
          boost::bind(&tcp_connection::handle_read, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)); 
  }

  void tcp_connection::handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred) {
      if(error) 
      return;
     std::cout.write(buf_.data(), bytes_transferred);
     std::cout << std::endl;
      //вместо этой функции(что ниже) вызывать бродкаст
      write(boost::asio::buffer(buf_, bytes_transferred)); //в сокет типу кидаем обратно(в его терминал)

    }
  void tcp_connection::handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
    read(boost::asio::buffer(buf_));
  }

