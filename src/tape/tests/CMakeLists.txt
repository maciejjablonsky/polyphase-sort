set(TARGET_NAME tape_test)
add_executable(${TARGET_NAME}  main.cpp Record_test.cpp TapeWriter_test.cpp)
target_link_libraries(${TARGET_NAME} tape gtest)
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Tests)

add_custom_command(
     TARGET ${TARGET_NAME}
     POST_BUILD
    #  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET_NAME}
     COMMENT "Running ${TARGET_NAME} ..."
     VERBATIM
     COMMAND $<$<CONFIG:Release>:${CMAKE_CTEST_COMMAND}>
     $<$<CONFIG:Release>:-C>
     $<$<CONFIG:Release>:Release>
     $<$<CONFIG:Release>:--output-on-failures>
     $<$<CONFIG:Release>:--verbose> # Uncomment this to see all passed tests.
)
