#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/json.hpp>

using boost::asio::ip::tcp;
std::string make_request(std::string nickname, std::string message) {
    boost::json::object obj;
    obj["nickname"] = nickname;
    obj["message"] = message;
    std::string s = boost::json::serialize(obj); //json text
    return s;
}

std::string make_message(std::string message) {
    boost::json::value jv = boost::json::parse(message);
    boost::json::object obj = jv.as_object();
    boost::json::value nickname = obj["nickname"];
    boost::json::value message1 = obj["message"];
    std::string result = std::string(nickname.as_string()) + ": " + message1.as_string().c_str();
    return result;
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: client <host>, <nickname>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "4567");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      std::array<char, 128> buf;
      boost::system::error_code error;
      std::string message;
      std::cin >> message;
      std::string result = make_request(argv[2], message);
      boost::asio::write(socket, boost::asio::buffer(result), error);
      if(error) 
        throw boost::system::system_error(error);
        
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
        

      std::cout  << make_message(buf.data()) << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
