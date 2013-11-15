cmake_minimum_required( VERSION 2.8 )

project( bratmobile )

add_subdirectory( externals )

include( ${CMAKE_BINARY_DIR}/externals.include )

#include( Boost REQUIRED )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}\ -std=c++11 )

include_directories( ${Boost_INCLUDE_DIR} )

add_executable( bratmobile
    src/main.cpp
)

target_link_libraries( bratmobile
    wiiuse
)
