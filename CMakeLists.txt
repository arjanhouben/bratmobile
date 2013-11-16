cmake_minimum_required( VERSION 2.8 )

project( bratmobile )

add_subdirectory( externals )

include( ${CMAKE_BINARY_DIR}/externals.include )

#include( Boost REQUIRED )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}\ -std=c++11\ -Wall )

include_directories(
    ${Boost_INCLUDE_DIR}
    include
)

add_executable( bratmobile
    src/main.cpp
    src/gpio.cpp
    include/gpio.h
)

add_executable( gpiotest
    src/gpiotest.cpp
    src/gpio.cpp
    include/gpio.h
)

if( UNIX )
    set( extra_libs pthread )
endif()

target_link_libraries( bratmobile
    wiiuse
    ${extra_libs}
)
