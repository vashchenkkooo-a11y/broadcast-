#include "auth_service.hpp"
#include "../shared/const.hpp"
#include <iostream>

// логика регистр авториз
auth_service::auth_service(client &client) : client_(client) {}

void auth_service::start()
{
    handle_menu();
}

void auth_service::handle_menu()
{ // в отдельный поток
    std::cout << "1 - Register " << std::endl
              << "2 - Log in" << std::endl
              << "3 - Send a message" << std::endl;

    std::string action;
    std::cin >> action;

    if (action == "1" || action == "2")
    {
        std::cout << "Enter your login" << std::endl;
        std::string login;
        std::cin >> login;
        std::cout << "Enter your password" << std::endl;
        std::string password;
        std::cin >> password;

        std::string request = serializer_.make_regauth_request(login, password, action == "1" ? Event::reg : Event::auth);

        client_.write(request);
        start();
    }
    else if (action == "3")
    {
        std::cout << "Enter the recipient's username" << std::endl;
        std::string nickname;
        std::cin >> nickname;
        std::cout << "Enter your message" << std::endl;
        std::string message;
        std::cin >> message;

        client_.write(serializer_.make_send_request(nickname, message));
        start();
    }
    else
    {
        std::cout << "Unknown command" << std::endl;
        start();
    }
}
// void auth_service::regauth( std::string &login,
//     std::string &password,
//     const std::string &event)
// {
//     serializer serializer;
//     std::string request =
//         serializer.make_regauth_request(login, password, event);

//     client_.write(request);
// }
