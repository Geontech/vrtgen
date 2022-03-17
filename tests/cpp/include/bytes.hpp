/*
 * Copyright (C) 2021 Geon Technologies, LLC
 *
 * This file is part of vrtgen.
 *
 * vrtgen is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef _BYTES_HPP
#define _BYTES_HPP

#include <vector>
#include <sstream>
#include <iomanip>

#include <catch.hpp>

/**
 * Typedef to abstract a comparison-capable data structure for use in testing
 * binary data, e.g.:
 *
 *     REQUIRE(value == bytes{1, 2, 3, 4});
 */
typedef std::vector<uint8_t> bytes;

/**
 * Utility function to get a subset of a byte array for comparisons:
 *
 *     REQUIRE(range(value, 4, 8) == bytes{1,2,3,4});
 */
inline bytes range(const bytes& data, size_t start, size_t end)
{
    return bytes(data.begin() + start, data.begin() + end);
}

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

#endif // _BYTES_HPP
