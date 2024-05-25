cmake_minimum_required(VERSION 3.0.0)
project(Ticket_system VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
include_directories(${CMAKE_SOURCE_DIR}/include)
enable_testing()
file(GLOB_RECURSE main_src include/*.cpp)
add_executable(code main.cpp ${main_src})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
# set cxx standard c++20
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
