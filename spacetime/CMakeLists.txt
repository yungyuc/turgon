# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>

cmake_minimum_required(VERSION 3.9)
project(spacetime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(pybind11 REQUIRED)
find_package(NumPy REQUIRED)
find_package(xtl REQUIRED)
find_package(xsimd REQUIRED)
find_package(xtensor REQUIRED)
find_package(xtensor-python REQUIRED)

include_directories(${xtensor_INCLUDE_DIRS})
include_directories(${NUMPY_INCLUDE_DIR})
include_directories("include")

set(SPACETIME_HEADERS
    # Overall.
    include/spacetime.hpp
    # Framework.
    include/spacetime/Celm.hpp
    include/spacetime/Celm_decl.hpp
    include/spacetime/ElementBase.hpp
    include/spacetime/ElementBase_decl.hpp
    include/spacetime/Grid.hpp
    include/spacetime/Grid_decl.hpp
    include/spacetime/Field.hpp
    include/spacetime/Field_decl.hpp
    include/spacetime/SolutionBase.hpp
    include/spacetime/SolutionBase_decl.hpp
    include/spacetime/Solution.hpp
    include/spacetime/Selm.hpp
    include/spacetime/Selm_decl.hpp
    include/spacetime/FelmBase.hpp
    include/spacetime/io.hpp
    include/spacetime/system.hpp
    include/spacetime/type.hpp
    # Physical kernels.
    include/spacetime/kernel/convection.hpp
    include/spacetime/kernel/inviscid_burgers.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       SPACETIME_HEADERS "${SPACETIME_HEADERS}")

set(EXAMPLE_SOURCES
    src/example.cpp
)
add_executable(example ${EXAMPLE_SOURCES} ${SPACETIME_HEADERS})

set(SPACETIME_PY_HEADERS
    include/spacetime/python.hpp
    include/spacetime/python/common.hpp
    include/spacetime/python/WrapBase.hpp
    include/spacetime/python/wrapper_spacetime.hpp
    include/spacetime/python/wrapper_convection.hpp
    include/spacetime/python/wrapper_inviscid_burgers.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       SPACETIME_PY_HEADERS "${SPACETIME_PY_HEADERS}")

set(LIBST_SOURCES
    src/python/libst.cpp
)
pybind11_add_module(
    _libst
    ${LIBST_SOURCES}
    ${SPACETIME_HEADERS}
    ${SPACETIME_PY_HEADERS}
)
install(TARGETS _libst DESTINATION libst)

# vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4:
