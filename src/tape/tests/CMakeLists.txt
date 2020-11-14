set(TARGET_NAME tape_test)
add_executable(${TARGET_NAME}
     main.cpp
     Record_test.cpp
     # TapeWriter_test.cpp
     PageWriter_test.cpp
     TestConfig.hpp
     TestConfig.cpp
     PageReader_test.cpp
     TapeReader_test.cpp
     Page_test.cpp
     RecordsPage_test.cpp
)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tmp)

target_link_libraries(${TARGET_NAME} tape gtest fmt)
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/tmp)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Tests)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/tmp")
add_custom_command(
     TARGET ${TARGET_NAME}
     POST_BUILD
     # WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET_NAME}
     COMMENT "Running ${TARGET_NAME} ..."
     VERBATIM
     COMMAND $<$<CONFIG:Release>:${CMAKE_CTEST_COMMAND}>
     $<$<CONFIG:Release>:-C>
     $<$<CONFIG:Release>:Release>
     $<$<CONFIG:Release>:--output-on-failures>
     $<$<CONFIG:Release>:--verbose> # Uncomment this to see all passed tests.
)
