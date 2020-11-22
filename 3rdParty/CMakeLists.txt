add_subdirectory(fmt)
set_target_properties(fmt PROPERTIES FOLDER 3rdParty)

add_subdirectory(googletest)
set_target_properties(gtest PROPERTIES FOLDER 3rdParty)
set_target_properties(gtest_main PROPERTIES FOLDER 3rdParty)
set_target_properties(gmock PROPERTIES FOLDER 3rdParty)
set_target_properties(gmock_main PROPERTIES FOLDER 3rdParty)