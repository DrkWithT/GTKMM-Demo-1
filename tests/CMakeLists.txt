add_executable(test_datatypes test_datatypes.cpp)
target_link_directories(test_datatypes PRIVATE "${CMAKE_HOME_DIRECTORY}/build")
target_link_libraries(test_datatypes PRIVATE datatypes)

add_test(NAME test_datatypes COMMAND "$<TARGET_FILE:test_datatypes>")
