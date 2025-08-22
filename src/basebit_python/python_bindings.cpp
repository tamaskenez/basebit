#include "basebit/basebit.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(basebit, m, py::mod_gil_not_used())
{
    m.doc() = "basebit"; // optional module docstring
    m.def("init", &basebit::init, "Initialize the basebit system");
    m.def(
      "set_interactive", &basebit::set_interactive, "Turn on to automatically update the window after each command"
    );
    m.def("create_window", &basebit::create_window, "Create the window and set a custom resolution and border size");
    m.def("background_color", &basebit::background_color);
    m.def("border_color", &basebit::border_color);
    m.def("clear", &basebit::clear);
    m.def("exec", &basebit::exec);
    py::class_<basebit::Resolution>(m, "Resolution")
      .def(py::init<int, int, int, int>())
      .def("full_width", &basebit::Resolution::full_width)
      .def("full_height", &basebit::Resolution::full_height)
      .def_readwrite("width", &basebit::Resolution::width)
      .def_readwrite("height", &basebit::Resolution::height)
      .def_readwrite("border_width", &basebit::Resolution::border_width)
      .def_readwrite("border_height", &basebit::Resolution::border_height);
}
