cmake_minimum_required(VERSION 3.16)
project(Broadcast LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS json)


add_executable(server
    server/main.cpp
    server/tcp_connection.cpp
    server/tcp_server.cpp
    shared/parser.cpp
    server/serializer.cpp
    shared/const.hpp 
)

add_executable(client 
    client/client.cpp
    client/serializer.cpp
    shared/parser.cpp
    client/main.cpp
    shared/const.hpp 
    client/auth_service.cpp
    )



target_link_libraries(client PRIVATE Boost::json)
target_link_libraries(server PRIVATE Boost::json)
