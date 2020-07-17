cmake_minimum_required(VERSION 3.15)
project(snake_clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(snake_clion main.cpp view.h view.cpp
                                    tui.h tui.cpp
                                    keypressable.h
                                    human.h human.cpp
                                    game.h game.cpp
                                    control.h
                                    ai.h ai.cpp)