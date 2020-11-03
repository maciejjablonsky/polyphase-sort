set(TARGET_NAME polyphase_sort)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC fmt)