set(TARGET_NAME tape_generator)
add_executable(${TARGET_NAME} main.cpp TapeGenerator.cpp TapeGenerator.hpp)
target_link_libraries(${TARGET_NAME} fmt tape)