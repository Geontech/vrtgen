/*
 * Copyright (C) 2022 Geon Technologies, LLC
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

const size_t CLASS_ID_BYTES = 8;
const size_t HEADER_BYTES = 4;
const size_t STREAM_ID_BYTES = 4;
const size_t CAM_BYTES = 4;
const size_t CIF_BYTES = 4;
const size_t MESSAGE_ID_BYTES = 4;
const size_t INTEGER_TS_BYTES = 4;
const size_t FRACTIONAL_TS_BYTES = 8;
const size_t TRAILER_BYTES = 4;
const size_t CIF0_BYTES = 4;
const size_t CIF1_BYTES = 4;
const size_t CIF7_BYTES = 4;
const size_t REF_POINT_ID_BYTES = 4;
const size_t BANDWIDTH_BYTES = 8;
const size_t IF_REF_FREQ_BYTES = 8;
const size_t RF_REF_FREQ_BYTES = 8;
const size_t RF_REF_FREQ_OFFSET_BYTES = 8;
const size_t IF_BAND_OFFSET_BYTES = 8;
const size_t REF_LEVEL_BYTES = 4;
const size_t GAIN_BYTES = 4;
const size_t OVER_RANGE_COUNT_BYTES = 4;
const size_t SAMPLE_RATE_BYTES = 8;
const size_t TIMESTAMP_ADJ_BYTES = 8;
const size_t TIMESTAMP_CAL_TIME_BYTES = 4;
const size_t TEMP_BYTES = 4;
const size_t DEVICE_ID_BYTES = 8;
const size_t STATE_EVENT_INDICATOR_BYTES = 4;
const size_t PAYLOAD_FORMAT_BYTES = 8;
const size_t EPHEM_REF_ID_BYTES = 4;
const size_t PHASE_OFFSET_BYTES = 4;
const size_t POLARIZATION_BYTES = 4;

const size_t BASIC_DATA_BYTES = HEADER_BYTES;
const size_t BASIC_CONTEXT_BYTES = HEADER_BYTES + STREAM_ID_BYTES + CIF_BYTES;
const size_t BASIC_CONTROL_BYTES = HEADER_BYTES + STREAM_ID_BYTES + CIF_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
const size_t BASIC_ACK_BYTES = HEADER_BYTES + STREAM_ID_BYTES + CAM_BYTES + MESSAGE_ID_BYTES;
