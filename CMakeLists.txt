cmake_minimum_required(VERSION 3.15)
project(SocketsClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(SocketsClient main.cpp ClientAPI.cpp ClientAPI.h)

target_link_libraries(SocketsClient ws2_32)
target_compile_options(SocketsClient PRIVATE /W2 /WX)