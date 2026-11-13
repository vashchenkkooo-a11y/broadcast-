#pragma once
#include <string>

class serializer {

public:
std::string make_regauth_request(std::string login, std::string password, std::string event_name);
std::string make_send_request(std::string nickname, std::string message);
std::string make_logout_request();
};
    