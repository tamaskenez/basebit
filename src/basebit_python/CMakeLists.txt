pybind11_add_module(basebit_python "${CMAKE_CURRENT_SOURCE_DIR}/python_bindings.cpp")
set_target_properties(basebit_python PROPERTIES OUTPUT_NAME "basebit")
target_link_libraries(basebit_python PRIVATE
	basebit
)
install(TARGETS basebit_python DESTINATION .)

# basebit (and basebit_python) don't have any (non-system) shared library dependencies.
# Building with install rpath has two effects:
# - .so is not modified after installation, no need to re-ad-hoc-sign it
# - install_name_tool doesn't complain about no LC_RPATH entries for the `-delete_rpath` command
set_target_properties(basebit_python PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

foreach(t basebit_python pybind11::headers)
	foreach(pn INCLUDE_DIRECTORIES INTERFACE_INCLUDE_DIRECTORIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES LINK_INTERFACE_LIBRARIES LINK_LIBRARIES INTERFACE_LINK_LIBRARIES IMPORTED_LINK_INTERFACE_LIBRARIES)
		get_target_property(p ${t} ${pn})
		message(STATUS "[${t}] ${pn}: ${p}")
	endforeach()
endforeach()
