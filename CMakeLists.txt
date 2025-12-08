cmake_minimum_required(VERSION 3.20)
project(cproxy)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include_directories(src/common/plog src/common/http-parser)

add_executable(cproxy src/main.cpp src/common/http-parser/http_parser.c)
