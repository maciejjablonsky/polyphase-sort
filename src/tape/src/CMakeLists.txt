set(SOURCES Record.hpp Record.cpp TapeWriter.hpp TapeWriter.cpp Tape.hpp Tape.cpp TapeHeaders.hpp PageWriter.hpp PageWriter.cpp IHardDriveAccessor.hpp)
add_library(tape ${SOURCES})
target_include_directories(tape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tape PUBLIC common fmt)