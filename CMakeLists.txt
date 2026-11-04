cmake_minimum_required(VERSION 3.16)
project(Broadcast LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS json)


add_executable(server
    server/main.cpp
    server/tcp_connection.cpp
    server/tcp_server.cpp
)

add_executable(client 
    client/client.cpp
    client/client.hpp
    client/main.cpp)

target_link_libraries(client PRIVATE Boost::json)
target_link_libraries(server PRIVATE Boost::json)
