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

tcp_connection::pointer tcp_connection::create(boost::asio::io_context &io_context, std::map<std::string, std::string> &data, std::map<std::string, tcp_connection::pointer> &active_connections)
{
  return pointer(new tcp_connection(io_context, data, active_connections));
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

tcp_connection::tcp_connection(boost::asio::io_context &io_context, std::map<std::string, std::string> &data, std::map<std::string, tcp_connection::pointer> &active_connections)
    : socket_(io_context), data_(data), active_connections_(active_connections)
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
  if (error) {
    if (!login_.empty()) active_connections_.erase(login_);
  return;
  }
  // buf_[bytes_transferred] = '\0';
 
  parser parser;
  boost::json::object obj_request;
  obj_request = parser.parse(std::string(buf_.data(), bytes_transferred));

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

  else if (event == Event::logout)
  {
    handle_logout();
  }
}
void tcp_connection::handle_write(const boost::system::error_code & error,
                                  size_t bytes_transferred)
{
  if (error) {
    if (!login_.empty()) active_connections_.erase(login_);
    return;
    }
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
  login_ = login; //чтобі потом писать  Миша: привет + знать, что чел которій хочет ОТПРАВИТЬ смс авториз уже
  active_connections_[login_] = shared_from_this();
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
  if(login_.empty()) {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_send_anonym);
    write(response_);
    return;
  }

  std::string nickname = data["nickname"].as_string().c_str();
  std::string message = data["message"].as_string().c_str();
  
  if(!active_connections_.count(nickname)) {
    response_ = serializer.make_response(Status::ERROR, Error_messages::error_send);
    write(response_);
    return;
  }

  std::string send =  serializer.make_response(Status::OK, login_ + ":" + message);
  active_connections_[nickname]->write(send);

  write(serializer.make_response(Status::OK, Success_messages::success_send));

}

void tcp_connection::handle_logout()
{
    serializer serializer;
    if (login_.empty()) {
        response_ = serializer.make_response(Status::ERROR, Error_messages::error_logout);
        write(response_);
        return;
    }
    active_connections_.erase(login_);
    login_.clear();
    response_ = serializer.make_response(Status::OK, Success_messages::success_logout);
    write(response_);
}
