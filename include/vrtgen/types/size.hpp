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

#ifndef _VRTGEN_TYPES_SIZE_HPP
#define _VRTGEN_TYPES_SIZE_HPP

namespace vrtgen {

namespace detail {

struct size_converter
{
    inline static constexpr std::size_t store(std::size_t value) noexcept
    {
        return value - 1;
    }

    inline static constexpr std::size_t load(std::size_t value) noexcept
    {
        return value + 1;
    }
}; // end struct size_converter

} // end namespace detail

} // end namespace vrtgen

#endif // _VRTGEN_TYPES_SIZE_HPP