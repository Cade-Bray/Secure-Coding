cmake_minimum_required(VERSION 3.29)
project(SQL_Injection)

set(CMAKE_CXX_STANDARD 20)

add_executable(SQL_Injection
        SQLInjection.cpp
        sqlite3.c)
