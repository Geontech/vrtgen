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

#include "bindump.hpp"

#include <iomanip>

void dump(std::ostream& oss, const unsigned char* buffer, size_t length)
{
    oss << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);
    for (unsigned index = 0; index < length; ++index) {
        if (index % 8 == 0) {
            if (index != 0) {
                oss << std::endl;
            }
            oss << "0x";
        }
        oss << std::setw(2) << static_cast<unsigned>(buffer[index]);
    }
    oss << std::endl;
}
