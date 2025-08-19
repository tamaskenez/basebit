file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.h *.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})
add_library(sdl_util STATIC ${sources})
target_link_libraries(sdl_util PRIVATE
	SDL3::SDL3
	meadow
)
target_include_directories(sdl_util PUBLIC .)
