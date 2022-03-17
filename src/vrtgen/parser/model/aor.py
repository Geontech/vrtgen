from dataclasses import dataclass

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import CIF

@dataclass
class Record(PackedStruct):
    """
    Base Record class for Array-of-Records field types
    """
    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__

@dataclass
class ArrayOfRecords(TemplateArrayStruct):
    name : str = 'array_of_records'
    array_size : Unsigned32 = field(default_factory=lambda: Unsigned32('array_size', packed_tag=PackedTag(31,32,0)))
    header_size : IntegerType = field(default_factory=lambda: IntegerType('header_size', bits=8, packed_tag=PackedTag(31,8,1,0)))
    num_words_record : IntegerType = field(default_factory=lambda: IntegerType('num_words_record', bits=12, packed_tag=PackedTag(23,12,1,0)))
    num_records : IntegerType = field(default_factory=lambda: IntegerType('num_records', bits=12, packed_tag=PackedTag(11,12,1,0)))
    packed_0 : PackedType = field(default_factory=lambda: PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,1)))
    subfield_cif : CIF = field(default_factory=lambda: CIF('subfield_cif', packed_tag=PackedTag(31,32,2)))
    records : ListType = field(default_factory=lambda: ListType('records', type_=Record, linked_size=num_records, packed_tag=PackedTag(31,0,3)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.total_size_field = self.array_size

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.fields]:
                raise ValueError('invalid field provided: ' + field)

    def _parse_mapping(self, mapping):
        pass

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)

    @property
    def is_array_of_records(self):
        return True
