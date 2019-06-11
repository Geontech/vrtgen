#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <vector>
#include <sstream>
#include <iomanip>

#include <catch/catch.hpp>

/**
 * Typedef to abstract a comparison-capable data structure for use in testing
 * binary data, e.g.:
 *
 *     REQUIRE(value == bytes({1, 2, 3, 4}));
 */
typedef std::vector<uint8_t> bytes;

namespace Catch {
    /**
     * Override string conversion for byte data to display hexadecimal values.
     */
    template <>
    struct StringMaker<bytes>
    {
        static std::string convert(const bytes& value)
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
