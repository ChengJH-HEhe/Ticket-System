cmake_minimum_required(VERSION 3.0.0)
project(Bplustree VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()
add_executable(code main.cpp)
# target_compile_options(Bplustree PUBLIC -fsanitize=address,undefined)
# add -fsanitize=address, undefined to the compile option
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
