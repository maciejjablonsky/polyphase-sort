set(SOURCES factory_functions.hpp dummy.cpp)
add_library(common ${SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

