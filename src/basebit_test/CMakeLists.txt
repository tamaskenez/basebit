file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.h *.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

add_executable(basebit_test ${sources})
target_link_libraries(basebit_test PRIVATE
	basebit
    meadow
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME basebit_test COMMAND basebit_test)

target_include_directories(basebit PRIVATE /Users/dk8TamKe/git/basebit/submodules/pybind11/include)

