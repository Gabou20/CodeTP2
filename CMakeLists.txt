cmake_minimum_required(VERSION 3.5)
project(CodeTP2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    Graphe.cpp
    Graphe.h
    Principal.cpp
    ReseauAerien.cpp
    ReseauAerien.h)

add_executable(CodeTP2 ${SOURCE_FILES})