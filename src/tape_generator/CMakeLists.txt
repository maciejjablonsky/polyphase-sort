set(TARGET_NAME tape_generator)
set(TARGET_SRC TapeGenerator.hpp TapeGenerator.cpp main.cpp)
add_executable(${TARGET_NAME} ${TARGET_SRC})
target_link_libraries(${TARGET_NAME} PRIVATE  fmt)