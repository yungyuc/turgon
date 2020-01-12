# Find the modmesh package
# modmesh_INCLUDE_DIR
# modmesh_FOUND
# will be set by this script

cmake_minimum_required(VERSION 2.8.11)

find_path(modmesh_INCLUDE_DIR NAMES modmesh/modmesh.hpp
  HINTS ${modmesh_ROOT} ENV PATH PATH_SUFFIXES include)

if(modmesh_INCLUDE_DIR)
  message(STATUS "modmesh_INCLUDE_DIR: ${modmesh_INCLUDE_DIR}")
  set(modmesh_FOUND 1 CACHE INTERNAL "modmesh found")
else()
  message(STATUS "modmesh_INCLUDE_DIR: not found")
endif()