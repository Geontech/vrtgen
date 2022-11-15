from dataclasses import dataclass, field

from vrtgen.parser.model.base import Packet, ListType


@dataclass
class InformationClass:
    code : int = 0
    packet_classes : ListType = field(default_factory=lambda: ListType('packet_classes', type_=Packet))

    def _validate(self, mapping):
        # for field in mapping:
        #     if not field in [f.name for f in self.fields]:
        #         raise ValueError('invalid information class field provided: ' + field)
        pass

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'packet_classes':
                self.__dict__[key].values = val
            else:
                self.__dict__[key] = val

    def validate_and_parse_mapping(self, **mapping):
        self._validate(mapping)
        self._parse_mapping(mapping)