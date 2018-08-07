cmake_minimum_required(VERSION 3.10)
project(FlashCards)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-lncurses -lpanel")

add_executable(FlashCards main.cpp activities.cpp activities.h ui.cpp ui.h ui_support.cpp ui_support.h objects.cpp objects.h constants.h app.h)