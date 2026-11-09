#pragma once 
#include <string>

namespace Status {
  inline const std::string OK = "ok";
  inline const std::string ERROR = "error";
}

namespace Success_messages {
  inline const std::string success_reg = "User registered";
  inline const std::string success_auth = "Login successful";
  inline const std::string success_send = "Message sent";
}

namespace Error_messages {
  inline const std::string error_reg = "login exist";
  inline const std::string error_auth = "Invalid username or password";
  inline const std::string error_send = "Recipient not found";
  inline const std::string error_send_anonym = "LOG IN!";
}

namespace Event {
  inline const std::string reg = "reg";
  inline const std::string auth = "auth";
  inline const std::string send = "send";
}