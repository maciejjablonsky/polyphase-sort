set(SOURCES TestConfig.hpp TestConfig.cpp)
add_library(test_config ${SOURCES})
target_link_libraries(test_config PUBLIC fmt common)
target_include_directories(test_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(test_config PROPERTIES FOLDER Tests)