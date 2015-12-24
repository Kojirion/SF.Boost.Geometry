project(SFGeometry)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
find_package(Boost REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_subdirectory(examples)
