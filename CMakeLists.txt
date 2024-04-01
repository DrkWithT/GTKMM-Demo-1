cmake_minimum_required(VERSION 3.27.7)

project("GTKDemo1" VERSION 0.1.0)

set(BUILD_DEBUG_MODE True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(BUILD_DEBUG_MODE)
    add_compile_options(-Wall -Wextra -Wpedantic -g -Og)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/build")

find_package(GTK2 2.2 REQUIRED gtk gtkmm)

if (GTK2_FOUND)
    include_directories("${CMAKE_HOME_DIRECTORY}/include" "${GTK2_INCLUDE_DIRS}")
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
