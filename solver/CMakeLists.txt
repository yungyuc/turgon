# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>

cmake_minimum_required(VERSION 3.9)
project(solver)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(xtl REQUIRED)
find_package(xsimd REQUIRED)
find_package(xtensor REQUIRED)
include_directories(${xtensor_INCLUDE_DIRS})

set(SOLVER_SOURCES
    solver.cpp
)
add_executable(solver ${SOLVER_SOURCES})

# vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4:
