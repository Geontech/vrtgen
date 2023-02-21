/*
 * Copyright (C) 2023 Geon Technologies, LLC
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

#pragma once

#include <concepts>
#include <cmath>
#include <cstdint>

namespace vrtgen::fixed {

/**
 * @fn to_int
 * @brief Convert floating-point value to it's fixed-point integer representation
 * @tparam N Total number of fixed-point integer bits
 * @tparam R Radix point
 * @param value Floating-point value to convert
 * @return Fixed-point integer representation of value
 */
template <std::size_t N, std::size_t R>
inline constexpr auto to_int(std::floating_point auto value) noexcept
{
    constexpr auto scale = static_cast<decltype(value)>(static_cast<uint64_t>(1) << R);
    if constexpr (N > 32) {
        return static_cast<int64_t>(std::round(value * scale));
    } else if constexpr(N > 16) {
        return static_cast<int32_t>(std::round(value * scale));
    } else if constexpr(N > 8) {
        return static_cast<int16_t>(std::round(value * scale));
    } else {
        return static_cast<int8_t>(std::round(value * scale));
    }
}

/**
 * @fn to_fp
 * @brief Convert fixed-point integer value to it's floating-point representation
 * @tparam N Total number of fixed-point integer bits
 * @tparam R Radix point
 * @param value Fixed-point integer value to convert
 * @return Floating-point representation of value
 */
template <std::size_t N, std::size_t R>
inline constexpr auto to_fp(std::integral auto value) noexcept
{
    constexpr auto scale = (static_cast<uint64_t>(1) << R);
    if constexpr (N >= 32) {
        return static_cast<long double>(value) / scale;
    } else {
        return static_cast<double>(value) / scale;
    }
}

} // end namepsace vrtgen::fixed
