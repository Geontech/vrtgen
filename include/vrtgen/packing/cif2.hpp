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
#ifndef _VRTGEN_PACKING_CIF2_HPP
#define _VRTGEN_PACKING_CIF2_HPP

#include <vrtgen/packing/indicator_fields.hpp>

namespace vrtgen::packing {

/**
 * @class CIF2
 * @brief Context/Command Indicator Field Word 2 (VITA 49.2-2017 Section 9.1)
 */
class CIF2 : public IndicatorField2 {};

} // end namespace vrtgen::packing

#endif // _VRTGEN_PACKING_CIF2_HPP
