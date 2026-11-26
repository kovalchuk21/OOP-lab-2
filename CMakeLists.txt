cmake_minimum_required(VERSION 3.16)

project(ATM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ATM main.cpp atm.cpp atm.h ui.h)

