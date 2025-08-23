#pragma once

#include "basebit/export.h"

#include <filesystem>
#include <map>
#include <string_view>
#include <utility>
#include <vector>

namespace basebit
{
/*
std::vector<std::vector<const char*>> charset_style_1 = {
  {"XXX.", //
   "X.X.", //
   "XXX."}, //
  {"X.X.", //
   ".X..", //
   "X.X" }, //
};

std::vector<std::pair<char, std::vector<const char*>>> charset_style_2 = {
  {'O',
   {"XXX.",   //
    "X.X.",   //
    "X.X."}}, //
  {'X',
   {"X.X.",   //
    ".X..",   //
    "X.X."}}, //
};

std::vector<std::pair<int, std::vector<const char*>>> charset_style_3 = {
  {1,
   {"XXX.",   //
    "X.X.",   //
    "X.X."}}, //
  {2,
   {"X.X.",   //
    ".X..",   //
    "X.X."}}, //
};

 In text file (CharsetFileFormat::Type::basebit_yaml)

- code: 1 # The character index can be specified as an integer.
  bitmap:
    - XXX.
    - X.X.
    - X.X.

- char: X # Or a character.
  bitmap:
    - X.X.
    - .X..
    - X.X.
*/

struct BASEBIT_EXPORT CharsetFileFormat {
    enum class Type {
        raw_bytes_of_8_pixel_wide_chars, // Specify height.
    } type;
    int width = 0;  // 0 is "auto".
    int height = 0; // 0 is "auto".
};

namespace detail
{
class CharsetDynamicBitset;
}

// Monochrome, monospaced character set.
class BASEBIT_EXPORT Charset
{
    int width = 0, height = 0;
    std::map<int, size_t> chars;
    std::unique_ptr<detail::CharsetDynamicBitset> bits;

public:
    static const Charset commodore_64_upper;
    static const Charset commodore_64_lower;

    static Charset from_file(const std::filesystem::path& p, const CharsetFileFormat& format);
    std::string write_as_cpp_source_code(std::string_view indentation = "\t") const;

    Charset(int w, int h);
    explicit Charset(std::vector<std::vector<const char*>> cs);
    explicit Charset(std::vector<std::pair<char, std::vector<const char*>>> cs);
    explicit Charset(const std::vector<std::pair<int, std::vector<const char*>>>& cs);

    Charset(Charset&&);

    ~Charset();
};

} // namespace basebit
