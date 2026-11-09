#include "client.hpp"
#include "serializer.hpp"
#include "../shared/parser.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include "../shared/const.hpp"
#include <boost/json.hpp>

using boost::asio::ip::tcp;
client::client(boost::asio::io_context &io_context, tcp::resolver::results_type &endpoints) : socket(io_context)
{
    boost::asio::connect(socket, endpoints);
    serializer serializer;
}

boost::json::object client::regauth(
    std::string &login,
    std::string &password,
    const std::string &event)
{
    serializer serializer;
    std::string request =
        serializer.make_regauth_request(login, password, event);

    std::array<char, 128> buf;
    boost::system::error_code error;

    boost::asio::write(socket, boost::asio::buffer(request), error);

    if (error)
        throw boost::system::system_error(error);

    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    buf[len] = '\0';

    if (error)
    throw boost::system::system_error(error);
    
    parser parser;
    boost::json::object response;
    response = parser.parse(std::string(buf.data(), len));

    if (response["status"].as_string() == Status::OK)
        std::cout << "круто" << std::endl;

    else
        std::cout << response["message"].as_string().c_str() << std::endl;
    return response;
}
