cmake_minimum_required(VERSION 3.9)

project(bcclaster VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)


add_executable(bcclaster src/module.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(bcclaster PUBLIC OpenMP::OpenMP_CXX)
endif()