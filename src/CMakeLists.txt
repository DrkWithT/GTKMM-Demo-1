add_executable(demoapp main.cpp)

# target_link_libraries(utils)
target_link_libraries(demoapp "${GTK2_LIBRARIES}" datatypes)
