#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "result/result.hpp"

using std::array;
using std::string_view;
using namespace std::literals;

using result_type::Err;
using result_type::Ok;
using result_type::Result;

enum class Version : uint8_t
{
    V1 = 1,
    V2 = 2
};

// Make Version debuggable (required by cpp-result)
std::ostream &operator<<(std::ostream &os, Version v) { return os << static_cast<int>(v); }

auto parse_version(array<uint8_t, 6> const &header) -> Result<Version, string_view>
{
    switch (header.at(0))
    {
    case 1:
        return Ok(Version::V1);
    case 2:
        return Ok(Version::V2);
    default:
        return Err("Unknown version"sv);
    }
}

auto parse_data(array<uint8_t, 6> const &header) -> Result<uint8_t, string_view>
{
    const Version version = TRY_OK(parse_version(header));
    return Ok<uint8_t>(static_cast<uint8_t>(version) + header[1] +
                       header[2]); // the + performs integer promotion
                                   // thus casting the final result to uint8_t is needed
}

int main()
{
    parse_version({2, 3, 4, 5, 6, 7})
        .match(
            [](auto version)
            {
                std::cout << "Version: " << static_cast<int>(version) << std::endl;
            },
            [](auto error)
            {
                std::cout << error << std::endl;
            });

    int const parsed = parse_data({2, 3, 4, 5, 6, 7}).unwrap();
    std::cout << "parsed: " << parsed << std::endl;
}
