cmake_minimum_required(VERSION 3.5)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/libs)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_subdirectory( apps )
add_subdirectory( libs )
add_subdirectory( thirdparty )