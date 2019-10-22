cmake_minimum_required(VERSION 3.10)

add_subdirectory(libuv)
add_subdirectory(GSL)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(uvw)
add_library(uvw_interface INTERFACE)
target_include_directories(uvw_interface SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uvw/src)
target_link_libraries(uvw_interface INTERFACE uv_a)

if(WIN32)
target_link_libraries(uvw_interface INTERFACE ws2_32)
endif(WIN32)