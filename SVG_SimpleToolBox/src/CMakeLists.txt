cmake_minimum_required(VERSION 3.5)

project(SmallToolBox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    test.cpp
    svgToolBox.hpp
)

set(RESOURCE_FILES
    test_commands.txt
)

add_executable(test ${SOURCES})

install(TARGETS test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Resources)
