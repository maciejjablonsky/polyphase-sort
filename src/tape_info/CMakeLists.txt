set(SOURCES main.cpp Info.hpp Info.cpp)
add_executable(tape_info ${SOURCES})
target_link_libraries(tape_info tape fmt)