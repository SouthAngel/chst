cmake_minimum_required(VERSION 3.15.0)
project(chst VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(chst main.cpp)
target_link_options(chst PRIVATE "LINKER:/NODEFAULTLIB:library")
# /NODEFAULTLIB:library
target_include_directories(chst PRIVATE libs/lua/include)
target_link_directories(chst PRIVATE libs/lua)
target_link_libraries(chst lua53)

