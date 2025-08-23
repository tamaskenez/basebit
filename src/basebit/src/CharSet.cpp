#include "basebit/Charset.h"

#include "basebit/Error.h"

#include <meadow/cppext.h>
#include <meadow/dynamic_bitset.h>

#include <SDL3/SDL_Surface.h>

#include <fstream>

namespace basebit
{
namespace detail
{
class CharsetDynamicBitset : public sul::dynamic_bitset<>
{
};
} // namespace detail

namespace
{
Charset from_vector_vector_cstring(vector<vector<const char*>> cs)
{
    std::vector<std::pair<int, std::vector<const char*>>> ds;
    ds.reserve(cs.size());
    unordered_set<char> used_indices;
    int index = 0;
    for (auto& csi : cs) {
        ds.emplace_back(pair(index++, MOVE(csi)));
    }
    return Charset(ds);
}

Charset from_vector_pair_char_vector_cstring(vector<pair<char, vector<const char*>>> cs)
{
    std::vector<std::pair<int, std::vector<const char*>>> ds;
    ds.reserve(cs.size());
    for (auto& csi : cs) {
        const auto unsigned_index = bitcast<unsigned char>(csi.first);
        ds.emplace_back(pair(iicast<int>(unsigned_index), MOVE(csi.second)));
    }
    return Charset(ds);
}
} // namespace

Charset Charset::from_file(const fs::path& path, const CharsetFileFormat& file_format)
{
    std::error_code ec;
    auto size = fs::file_size(path, ec);
    if (ec) {
        throw Error(format("Can't get file size for \"{}\", reason: {}", path.string(), ec.message()));
    }
    switch (file_format.type) {
    case CharsetFileFormat::Type::raw_bytes_of_8_pixel_wide_chars: {
        if (file_format.height <= 0) {
            throw Error("CharsetFileFormat::Type::raw_bytes_of_8_pixel_wide_chars requires CharsetFileFormat::height "
                        "to be positive");
        }
        if (file_format.width != 0 && file_format.width != 8) {
            throw Error("CharsetFileFormat::Type::raw_bytes_of_8_pixel_wide_chars requires CharsetFileFormat::with to "
                        "be 0 (auto) or 8");
        }
        if (size % sucast(file_format.height) != 0) {
            throw Error(format("File size of \"{}\" is not divisible by height ({})", size, file_format.height));
        }
        const int num_chars = iicast<int>(size) / file_format.height;
        vector<char> contents(size);
        std::ifstream f(path, std::ios_base::in | std::ios_base::binary);
        if (!f) {
            throw Error(format("Can't open file \"{}\" for reading", path.string()));
        }
        f.read(contents.data(), iicast<std::streamsize>(size));
        if (cmp_not_equal(f.gcount(), size)) {
            throw Error(
              format("Can't read entire file \"{}\" of size {}, read only {}", path.string(), size, f.gcount())
            );
        }
        Charset charset(8, file_format.height);
        for (int code = 0; code < num_chars; ++code) {
            charset.chars.emplace(code, charset.bits->size());
            for (int row_ix : vi::iota(0, 8)) {
                auto row = contents[sucast(code * 8 + row_ix)];
                for (int col_ix = 7; col_ix >= 0; --col_ix) {
                    charset.bits->push_back((row & (1 << col_ix)) != 0);
                }
            }
        }
        return charset;
    }
    }
}

Charset::Charset(int w, int h)
    : bits(make_unique<detail::CharsetDynamicBitset>())
    , width_(w)
    , height_(h)
{
}

Charset::Charset(Charset&&) = default;

Charset::~Charset() = default;

Charset::Charset(vector<vector<const char*>> cs)
    : Charset(from_vector_vector_cstring(MOVE(cs)))
{
}

Charset::Charset(vector<pair<char, vector<const char*>>> cs)
    : Charset(from_vector_pair_char_vector_cstring(MOVE(cs)))
{
}

Charset::Charset(const std::vector<std::pair<int, std::vector<const char*>>>& cs)
    : bits(make_unique<detail::CharsetDynamicBitset>())
{
    for (size_t i : vi::iota(0u, cs.size())) {
        auto char_code = cs[i].first;
        auto& csi = cs[i].second;
        if (width_ > 0) {
            if (cmp_not_equal(csi.size(), height_)) {
                throw Error(
                  format("Char #{} has {} rows which is different from char #0 which has {}", i, csi.size(), height_)
                );
            }
        } else {
            width_ = iicast<int>(csi.size());
            height_ = iicast<int>(strlen(csi[0]));
            if (width_ == 0) {
                throw Error(format("Char #{}, the first row is empty", i));
            }
            if (height_ == 0) {
                throw Error(format("Char #{} has no rows", i));
            }
        }
        size_t first_bit_ix = bits->size();
        for (size_t r : vi::iota(0u, csi.size())) {
            auto& csi_r = csi[r];
            size_t wr = strlen(csi_r);
            if (cmp_not_equal(wr, width_)) {
                throw Error(
                  format("Character #{}, lenght of row #{} is {} while previous rows are {} long", i, r, wr, width_)
                );
            }
            for (size_t c : vi::iota(0u, wr)) {
                const char ch = csi_r[c];
                bits->push_back(ch != '.' && ch != ' ');
            }
        }
        if (!chars.emplace(char_code, first_bit_ix).second) {
            throw Error(format(
              "Character set has duplicate indices: index#{} ({})",
              char_code,
              std::in_range<unsigned char>(char_code) && isprint(char_code)
                ? format("'{}'", iicast<unsigned char>(char_code))
                : format("{:#02x}", char_code)
            ));
        }
    }
}

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

string Charset::write_as_cpp_source_code(string_view indentation) const
{
    string s = "std::vector<std::pair<int, std::vector<const char*>>> charset = {\n";
    for (auto& [k, v] : chars) {
        s += format(
          "{} {{{}, // {}\n",
          indentation,
          k,
          std::in_range<unsigned char>(k) && isprint(k) ? format("'{}'", iicast<unsigned char>(k))
                                                        : format("{:#02x}", k)
        );
        for (int row_ix : vi::iota(0, height_)) {
            string row_string;
            for (int col_ix : vi::iota(0, 8)) {
                row_string += bits->test(v + sucast(row_ix * width_ + col_ix)) ? 'X' : '.';
            }
            s += format(
              "{}  {}\"{}\"{} //\n",
              indentation,
              row_ix == 0 ? '{' : ' ',
              row_string,
              row_ix + 1 == height_ ? "}}," : ",  "
            );
        }
    }
    s += "};\n";
    return s;
}

namespace detail
{
std::unordered_map<int, int> write_charset_into_sdl_surface(const Charset& cs, SDL_Surface* surface)
{
    if (surface->w != cs.width()) {
        throw Error(format("SDL_Surface::w ({}) and Charset::width() ({}) must match", surface->w, cs.width()));
    }
    const int required_height = cs.height() * iicast<int>(cs.size());
    if (surface->h < required_height) {
        throw Error(format(
          "SDL_Surface::h ({}) must not be less than Charset::height() * Charset::size() ({} * {} = {})",
          surface->h,
          cs.height(),
          cs.size(),
          required_height
        ));
    }
    int surface_next_y0 = 0;
    std::unordered_map<int, int> code_to_surface_y0;
    for (auto& [code, bits_ix0] : cs.chars) {
        const int surface_y0 = surface_next_y0;
        surface_next_y0 += cs.height();
        code_to_surface_y0.emplace(code, surface_y0);
        uint8_t* const pixels_byte0 = reinterpret_cast<uint8_t*>(surface->pixels) + surface_y0 * surface->pitch;
        for (int char_y : vi::iota(0, cs.height())) {
            const auto bits_row_ix = bits_ix0 + iicast<size_t>(char_y * cs.width());
            uint8_t* const pixels_row_byte = pixels_byte0 + char_y * surface->pitch;
            const int end_row_byte_ix = (cs.width() + 7) / 8;
            for (int row_byte_ix = 0; row_byte_ix < end_row_byte_ix; ++row_byte_ix) {
                uint8_t byte = 0;
                const int byte_begin_x = row_byte_ix * 8;
                const int byte_end_x = std::min(byte_begin_x + 8, cs.width());
                for (int char_x = byte_begin_x; char_x < byte_end_x; ++char_x) {
                    const int x_within_byte = char_x % 8;
                    const int bit_ix = 7 - x_within_byte;
                    if (cs.bits->test(bits_row_ix + iicast<size_t>(char_x))) {
                        byte |= (1 << bit_ix);
                    }
                }
                pixels_row_byte[row_byte_ix] = byte;
            }
        }
    }
    return code_to_surface_y0;
}
} // namespace detail

} // namespace basebit
