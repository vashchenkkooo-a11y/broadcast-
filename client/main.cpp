#include "serializer.hpp"
#include "../shared/parser.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include "../shared/const.hpp"
#include "client.hpp"
#include "auth_service.hpp"

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
        resolver.resolve(argv[1], "4567");

    // tcp::socket socket(io_context);
    // boost::asio::connect(socket, endpoints);
    // serializer serializer;
    client client_(io_context);
    client_.connect(endpoints);
    auth_service auth(client_);

    while (true)
    {
      std::cout << "1 - регистрация " << std::endl
                << "2 - залогиниться" << std::endl;
      std::string action;
      std::cin >> action;

      if (action != "1" && action != "2")
      {
        return 1;
      }

      std::cout << "введите логин" << std::endl;
      std::string login;
      std::cin >> login;
      std::cout << "введите пароль" << std::endl;
      std::string password;
      std::cin >> password;
      boost::json::object response = auth.regauth(login, password, action == "1" ? Event::reg : Event::auth);
    }
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
