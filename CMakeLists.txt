cmake_minimum_required( VERSION 2.8 )

project( bratmobile )

include( Boost )

include_directories( ${Boost_INCLUDE_DIR} )

add_executable( bratmobile
    src/main.cpp
)
