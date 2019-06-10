#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <array>

#include <catch/catch.hpp>

/**
 * Utility function to create an array of bytes from a variable argument list
 * of values. Useful for creating comparison-capable data structures in a
 * test condition, e.g.:
 *
 *     REQUIRE(value == bytes(1,2,3,4));
 *
 * Due to limitations in argument handling, integer constants are promoted to
 * at least `int`, so range-checking on conversion to `uint8_t` is lost.
 */
template <typename... Args>
std::array<uint8_t, sizeof...(Args)> bytes(const Args&&... args)
{
    return { static_cast<uint8_t>(args)... };
}

namespace Catch {
    /**
     * Override string conversion for std::array with an element type of
     * `uint8_t` to display as a hexadecimal value.
     */
    template <size_t N>
    struct StringMaker< std::array<uint8_t, N> >
    {
        static std::string convert(const std::array<uint8_t, N>& value)
        {
            std::ostringstream oss;
            oss << "0x" << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);
            for (auto item : value) {
                oss << std::setw(2) << static_cast<unsigned>(item);
            }
            return oss.str();
        }
    };
}

#endif // _UTILS_HPP
