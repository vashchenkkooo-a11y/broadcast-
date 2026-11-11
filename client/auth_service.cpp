#include "auth_service.hpp"
#include "serializer.hpp"
#include "../shared/parser.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include "../shared/const.hpp"
#include <boost/json.hpp>
#include "client.hpp"

// логика регистр авториз
auth_service::auth_service(client &client) : client_(client) {}

boost::json::object auth_service::regauth(
    std::string &login,
    std::string &password,
    const std::string &event)
{

    serializer serializer;
    std::string request = serializer.make_regauth_request(login, password, event);

    parser parser;
    client_.write(request);
    boost::json::object response = parser.parse(client_.read());

    if (response["status"].as_string() == Status::OK)
        std::cout << (event == Event::reg ? Success_messages::success_reg : Success_messages::success_auth) << std::endl;
    else
        std::cout << response["message"].as_string().c_str() << std::endl;

    return response;
}
