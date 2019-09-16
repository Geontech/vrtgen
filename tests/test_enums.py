# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

from collections import namedtuple

import pytest

from vrtgen.types.enums import PacketType

PacketTypeTraits = namedtuple('PacketTypeTraits', 'data context command extension')

def _packet_type_param(packet_type, *args):
    return pytest.param(packet_type, PacketTypeTraits(*args), id=packet_type.name)

@pytest.mark.parametrize(
    "input,traits",
    [
        _packet_type_param(PacketType.SIGNAL_DATA_STREAM_ID, True, False, False, False),
        _packet_type_param(PacketType.CONTEXT, False, True, False, False),
        _packet_type_param(PacketType.COMMAND, False, False, True, False),
        _packet_type_param(PacketType.EXTENSION_DATA, True, False, False, True),
    ],
)
def test_packet_type_attributes(input, traits):
    assert input.is_data == traits.data
    assert input.is_context == traits.context
    assert input.is_command == traits.command
    assert input.is_extension == traits.extension
