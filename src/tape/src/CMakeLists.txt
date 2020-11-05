set(TARGET_NAME tape)
add_library(${TARGET_NAME}  Record.hpp Record.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)