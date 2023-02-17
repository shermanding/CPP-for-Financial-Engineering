cmake_minimum_required(VERSION 3.10)

project(L3_book)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(util)
add_subdirectory(book)
add_subdirectory(csv_l3_md)
