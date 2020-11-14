set(SOURCES Distributor.hpp Distributor.cpp)
add_library(sorter ${SOURCES})
target_include_directories(sorter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sorter PUBLIC tape fmt)