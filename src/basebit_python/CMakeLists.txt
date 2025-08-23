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
