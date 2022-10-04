# Copyright (C) 2022 Geon Technologies, LLC
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

import yaml
import pytest
from helpers.utils import parse_document
from vrtgen.parser.loader import get_loader
from vrtgen.parser.model.types.enums import *

def test_class_id_no_mapping():
    with pytest.raises(ValueError) as e_info:
        document = """class_id: !ClassID"""
        parse_document(document)

def test_class_id_oui():
    document = """
    class_id: !ClassID
        oui: FF-EE-DD
    """
    name,class_id = parse_document(document)
    assert name == 'class_id'
    # Class ID
    assert class_id.name == 'class_id'
    assert class_id.type_ == 'ClassIdentifier'
    assert class_id.bits == 64
    assert class_id.enabled
    assert class_id.required
    # Pad Bits
    assert class_id.pad_bits.enabled
    assert class_id.pad_bits.required
    assert not class_id.pad_bits.value
    # OUI
    assert class_id.oui.enabled
    assert class_id.oui.required
    assert class_id.oui.value == 0xFFEEDD
    assert str(class_id.oui) == "FF-EE-DD"
    # Information code
    assert class_id.information_code.enabled
    assert class_id.information_code.required
    assert not class_id.information_code.value
    # Packet Code
    assert class_id.packet_code.enabled
    assert class_id.packet_code.required
    assert not class_id.packet_code.value

def test_class_id_packet_code():
    document = """
    class_id: !ClassID
        packet_code: 0x1234
    """
    name,class_id = parse_document(document)
    assert name == 'class_id'
    assert class_id.name == 'class_id'
    assert class_id.type_ == 'ClassIdentifier'
    assert class_id.bits == 64
    assert class_id.enabled
    assert class_id.required
    # Pad Bits
    assert class_id.pad_bits.enabled
    assert class_id.pad_bits.required
    assert not class_id.pad_bits.value
    # OUI
    assert class_id.oui.enabled
    assert class_id.oui.required
    assert not class_id.oui.value
    assert str(class_id.oui) == ''
    # Information code
    assert class_id.information_code.enabled
    assert class_id.information_code.required
    assert not class_id.information_code.value
    # Packet_code 
    assert class_id.packet_code.enabled
    assert class_id.packet_code.required
    assert class_id.packet_code.value == 0x1234

def test_class_id_invalid_key():
    document = """
    class_id: !ClassID
        invalid_key: 0x1234
    """
    with pytest.raises(KeyError) as e_info:
        parse_document(document)

def test_class_id_invalid_oui():
    document = """
    class_id: !ClassID
        oui: FF-EE
    """
    with pytest.raises(ValueError) as e_info:
        parse_document(document)