set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.h *.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})
add_library(basebit SHARED ${sources})
target_link_libraries(basebit
	PRIVATE
		SDL3::SDL3
		meadow
		sdl_util
)

include(GenerateExportHeader)
generate_export_header(basebit
	EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/basebit/export.h"
)
target_include_directories(basebit
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

