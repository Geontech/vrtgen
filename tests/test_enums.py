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
