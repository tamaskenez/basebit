#include "basebit/basebit.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(basebit, m, py::mod_gil_not_used())
{
    m.doc() = "basebit"; // optional module docstring
    m.def("init", &basebit::init, "Initialize the basebit system");
    m.def("set_interactive", &basebit::set_interactive, "Enable automatically updating the window after each command");
    m.def("create_window", &basebit::create_window, "Create the window and set a custom resolution and border size");
    m.def("border_color", py::overload_cast<const basebit::Color&>(&basebit::border_color));
    m.def("border_color", py::overload_cast<int>(&basebit::border_color));
    m.def("background_color", py::overload_cast<const basebit::Color&>(&basebit::background_color));
    m.def("background_color", py::overload_cast<int>(&basebit::background_color));
    m.def("color", py::overload_cast<const basebit::Color&>(&basebit::color));
    m.def("color", py::overload_cast<int>(&basebit::color));
    m.def("clear", &basebit::clear);
    m.def("palette", &basebit::palette);
    m.def("plot", &basebit::plot);
    m.def("exec", &basebit::exec);
    py::class_<basebit::Resolution>(m, "Resolution")
      .def(py::init<int, int, int, int>())
      .def("full_width", &basebit::Resolution::full_width)
      .def("full_height", &basebit::Resolution::full_height)
      .def_readwrite("width", &basebit::Resolution::width)
      .def_readwrite("height", &basebit::Resolution::height)
      .def_readwrite("border_width", &basebit::Resolution::border_width)
      .def_readwrite("border_height", &basebit::Resolution::border_height)
      .def_readonly_static("commodore_64", &basebit::Resolution::commodore_64)
      .def_readonly_static("zx_spectrum", &basebit::Resolution::zx_spectrum);
    py::class_<basebit::Palette>(m, "Palette")
      .def_readonly_static("commodore_64", &basebit::Palette::commodore_64)
      .def_readonly_static("zx_spectrum", &basebit::Palette::zx_spectrum);
    py::class_<basebit::Color>(m, "Color")
      .def(py::init<>())
      .def(
        py::init<int, int, int, int>(),
        "Initialize with sRGB values (0-255) and an optional alpha, which defaults to 255",
        py::arg("r"),
        py::arg("g"),
        py::arg("b"),
        py::arg("a") = 255
      )
      .def(
        py::init<std::string_view>(),
        "Initialize with an sRGB color code of 6 or 8 hex digits, with an optional '#' prefix"
      );
    py::register_local_exception<basebit::Error>(module, "Error", PyExc_RuntimeError);
}
