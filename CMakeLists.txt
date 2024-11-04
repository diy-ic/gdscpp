cmake_minimum_required(VERSION 3.13.0)
project(gdscpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  include/gdscpp/gdsCpp.hpp
  include/gdscpp/gdsForge.hpp
  include/gdscpp/gdsParser.hpp
  src/gdsCpp.cpp
  src/gdsParser.cpp
  src/gdsForge.cpp
  src/gdsImport.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})

# Ensures that the header files of the project is included
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include/gdscpp
)