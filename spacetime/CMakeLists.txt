# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>

cmake_minimum_required(VERSION 3.9)
project(spacetime)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands")

option(BUILD_GTESTS "build libst google-test suite" ON)
option(FORCE_LINT "force linters" OFF)
option(USE_CLANG_TIDY "use clang-tidy" OFF)
option(LINT_AS_ERRORS "clang-tidy warnings as errors" OFF)

if(FORCE_LINT AND NOT USE_CLANG_TIDY)
    message(STATUS "USE_CLANG_TIDY is forced to ON because FORCE_LINT is ON")
    set(USE_CLANG_TIDY ON)
endif()
find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found")
    if(FORCE_LINT)
        message(FATAL_ERROR "FORCE_LINT is on but clang-tidy is not found")
    endif()
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    if(USE_CLANG_TIDY OR FORCE_LINT)
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
        if(LINT_AS_ERRORS)
            set(DO_CLANG_TIDY "${DO_CLANG_TIDY}" "-warnings-as-errors=*")
        endif()
        message(STATUS "use clang-tidy with DO_CLANG_TIDY: ${DO_CLANG_TIDY}")
    else()
        message(STATUS "not use clang-tidy")
    endif()
endif()

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
    include/spacetime/Selm.hpp
    include/spacetime/Selm_decl.hpp
    include/spacetime/SolverBase.hpp
    include/spacetime/SolverBase_decl.hpp
    include/spacetime/Solver.hpp
    include/spacetime/io.hpp
    include/spacetime/system.hpp
    include/spacetime/type.hpp
    # Physical kernels.
    include/spacetime/kernel/linear_scalar.hpp
    include/spacetime/kernel/inviscid_burgers.hpp
)
string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/"
       SPACETIME_HEADERS "${SPACETIME_HEADERS}")

set(SPACETIME_PY_HEADERS
    include/spacetime/python.hpp
    include/spacetime/python/common.hpp
    include/spacetime/python/WrapBase.hpp
    include/spacetime/python/wrapper_spacetime.hpp
    include/spacetime/python/wrapper_linear_scalar.hpp
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

if(CLANG_TIDY_EXE AND USE_CLANG_TIDY)
    set_target_properties(
        _libst PROPERTIES
        CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
    )
endif()

if(BUILD_GTESTS)
    add_subdirectory(gtests)
endif()

# vim: set ff=unix fenc=utf8 nobomb et sw=4 ts=4:
