cmake_minimum_required(VERSION 3.20)
project(ft_containers)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

include_directories(.)

add_executable(test
        algorithm.hpp
        iterator.hpp
        main.cpp
        map.hpp
        set.hpp
        stack.hpp
        type_traits.hpp
        utility.hpp
        vector.hpp)
