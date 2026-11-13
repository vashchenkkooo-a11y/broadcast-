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
 
    auto wg = boost::asio::make_work_guard(io_context);
    auto th = std::make_unique<std::thread>([&](){io_context.run();});

       auth.start(); 
       
    if (th->joinable()) {
      th->join();
    }
  } 
    catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
    }

    return 0;
  }
