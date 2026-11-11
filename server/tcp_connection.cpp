#include "tcp_connection.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>
#include "../shared/parser.hpp"
#include "serializer.hpp"
#include "../shared/const.hpp"

void tcp_connection::start()
{
  read();
}

tcp_connection::pointer tcp_connection::create(boost::asio::io_context &io_context, std::map<std::string, std::string> &data)
{
  return pointer(new tcp_connection(io_context, data));
}
tcp::socket &tcp_connection::socket()
{
  return socket_;
}

void tcp_connection::write(const std::string &response)
{
  boost::asio::async_write(socket_, boost::asio::buffer(response),
                           boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
}

tcp_connection::tcp_connection(boost::asio::io_context &io_context, std::map<std::string, std::string> &data)
    : socket_(io_context), data_(data)
{
}

void tcp_connection::read()
{
  socket_.async_read_some(boost::asio::buffer(buf_),
                          boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_read(const boost::system::error_code &error, std::size_t bytes_transferred)
{
  if (error)
    return;
  // buf_[bytes_transferred] = '\0';
  parser parser;
  boost::json::object obj_request;
  obj_request = parser.parse(buf_.data());

  boost::json::string event = obj_request["event"].as_string();
  boost::json::object data = obj_request["data"].as_object();

  if (event == Event::reg)
  {
    handle_reg(data);
  }

  else if (event == Event::auth)
  {
    handle_auth(data);
  }

  else if (event == Event::send)
  {
    handle_send(data);
  }
}
void tcp_connection::handle_write(const boost::system::error_code & /*error*/,
                                  size_t /*bytes_transferred*/)
{
  read();
}

void tcp_connection::handle_reg(boost::json::object &data)
{

  serializer serializer;
  std::string login = data["login"].as_string().c_str();
  std::string password = data["password"].as_string().c_str();

  if (data_.count(login))
  {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_reg);
    write(response_);
    return;
  }

  data_[login] = password;
  response_ = serializer.make_response(Status::OK, Success_messages::success_reg);
  write(response_);
}

void tcp_connection::handle_auth(boost::json::object &data)
{
  serializer serializer;
  std::string login = data["login"].as_string().c_str();
  std::string password = data["password"].as_string().c_str();

  if (!data_.count(login))
  {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_auth);
    write(response_);
    return;
  }

  if (data_[login] != password)
  {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_auth);
    write(response_);
    return;
  }
 // если всё ок - запоминаем что авторизован
  login_ = login;
  response_ = serializer.make_response(Status::OK, Success_messages::success_auth);
  write(response_);
}
  // for(const std::string &l  : logins_) {
  //   if (l == login ) {
  //     response_ = serializer.make_response(Status::ERROR, Error_messages::error_reg);
  //     write(response_);
  //     return;
  //   }

  
void tcp_connection::handle_send(boost::json::object &data)
{
  serializer serializer;
  if (login_.size() == 0)
  {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_send_anonym);
    // std::memcpy(buf_.data(), response.data(), response.size());

    // buf_[response.size()] = '\0';
    // write(response.size());
    write(response_);
  }
}
