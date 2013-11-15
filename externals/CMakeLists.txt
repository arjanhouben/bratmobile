
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/build )

if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/build/Makefile )
    execute_process(
        COMMAND
            ${CMAKE_COMMAND} -DBUILD_EXAMPLE=NO -DBUILD_EXAMPLE_SDL=NO -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/wiiuse/install ${CMAKE_CURRENT_SOURCE_DIR}/wiiuse
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/build
    )
endif()

execute_process(
    COMMAND
        ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/build --target install
)

file(
    WRITE
        ${CMAKE_BINARY_DIR}/externals.include
"
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/install/include )
link_directories( ${CMAKE_CURRENT_BINARY_DIR}/wiiuse/install/lib )
"
)
