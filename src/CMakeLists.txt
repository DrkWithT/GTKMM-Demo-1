add_executable(demoapp main.cpp)

add_subdirectory(datatypes)

# target_link_libraries(utils)
target_link_directories(demoapp PRIVATE "${CMAKE_HOME_DIRECTORY}/build")
target_link_libraries(demoapp "${GTK2_LIBRARIES}" datatypes)
