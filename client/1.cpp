#include "auth_service.hpp"
#include "serializer.hpp"
#include "../shared/parser.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include "../shared/const.hpp"
#include <boost/json.hpp>
#include "client.hpp"


boost::json::object auth_service::regauth(
    std::string &login,
    std::string &password,
    const std::string &event)
{
    serializer serializer;
    std::string request =
        serializer.make_regauth_request(login, password, event);



    client.write(socket, boost::asio::buffer(request), error);

    if (error)
        throw boost::system::system_error(error);

    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    

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
