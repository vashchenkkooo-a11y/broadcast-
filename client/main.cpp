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
  boost::asio::io_context io_context;
  std::thread *th = nullptr;

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

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
    th = new std::thread([&]()
                         { io_context.run(); });

    auth.start();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  io_context.stop();
  if (th->joinable())
  {
    th->join();
  }
  delete th;
  return 0;
}
