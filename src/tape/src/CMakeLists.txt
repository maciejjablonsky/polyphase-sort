set(SOURCES
    Record.hpp
    Record.cpp
    TapeWriter.hpp
    TapeWriter.cpp
    Tape.hpp
    Tape.cpp
    TapeHeaders.hpp
    PageWriter.hpp
    PageWriter.cpp
    IHardDriveAccessor.hpp
    PageReader.hpp
    PageReader.cpp
    Page.hpp
    Page.cpp
    TapeReader.hpp
    TapeReader.cpp
    RecordsPage.hpp
    RecordsPage.cpp
)

add_library(tape ${SOURCES})
target_include_directories(tape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tape PUBLIC common fmt)