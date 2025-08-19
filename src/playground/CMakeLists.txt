file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.h *.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})
add_executable(playground MACOSX_BUNDLE ${sources})
target_link_libraries(playground PRIVATE
	basebit
	SDL3::SDL3
	sdl_util
	meadow
)
