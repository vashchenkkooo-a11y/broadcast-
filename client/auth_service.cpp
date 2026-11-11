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

void auth_service::regauth( std::string &login,
    std::string &password,
    const std::string &event)
{
    serializer serializer;
    std::string request =
        serializer.make_regauth_request(login, password, event);

    client_.write(request);
}