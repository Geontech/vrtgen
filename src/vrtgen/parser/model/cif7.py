from dataclasses import dataclass

from vrtgen.parser.model.types import *
from vrtgen.parser.model.base import CIF
from vrtgen.parser.value import parse_enable

@dataclass
class Probability(PackedStruct):
    """
    Probability Field
    """
    name                 : str = 'probability'
    probability_function : Unsigned8 = field(default_factory=lambda: Unsigned8('probability_function', enabled=True, required=True, packed_tag=PackedTag(15,8,0,0)))
    probability_percent  : Unsigned8 = field(default_factory=lambda: Unsigned8('probability_percent', enabled=True, required=True, packed_tag=PackedTag(7,8,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32

    @property
    def is_probability_type(self):
        return True

@dataclass
class Belief(PackedStruct):
    """
    Belief Field
    """
    name    : str = 'belief'
    belief_percent  : Unsigned8 = field(default_factory=lambda: Unsigned8('belief_percent', enabled=True, required=True, packed_tag=PackedTag(7,8,0,0)))

    def __post_init__(self):
        super().__post_init__()
        self.type_ = type(self).__name__
        self.bits = 32
        
    @property
    def is_belief_type(self):
        return True

@dataclass
class CIF7Attributes(TemplateArrayStruct):
    """
    CIF7 Attributes
    """
    name : str = 'cif_7_attributes'
    current_value : CIFEnableType = field(default_factory=lambda: CIFEnableType('current_value', template_type=True, indicator_only=True, packed_tag=PackedTag(31,1,0,0)))
    mean_value : CIFEnableType = field(default_factory=lambda: CIFEnableType('mean_value', template_type=True, packed_tag=PackedTag(30,1,0,0)))
    median_value : CIFEnableType = field(default_factory=lambda: CIFEnableType('median_value', template_type=True, packed_tag=PackedTag(29,1,0,0)))
    standard_deviation : CIFEnableType = field(default_factory=lambda: CIFEnableType('standard_deviation', template_type=True, packed_tag=PackedTag(28,1,0,0)))
    max_value : CIFEnableType = field(default_factory=lambda: CIFEnableType('max_value', template_type=True, packed_tag=PackedTag(27,1,0,0)))
    min_value : CIFEnableType = field(default_factory=lambda: CIFEnableType('min_value', template_type=True, packed_tag=PackedTag(26,1,0,0)))
    precision : CIFEnableType = field(default_factory=lambda: CIFEnableType('precision', template_type=True, packed_tag=PackedTag(25,1,0,0)))
    accuracy : CIFEnableType = field(default_factory=lambda: CIFEnableType('accuracy', template_type=True, packed_tag=PackedTag(24,1,0,0)))
    first_derivative : CIFEnableType = field(default_factory=lambda: CIFEnableType('first_derivative', template_type=True, packed_tag=PackedTag(23,1,0,0)))
    second_derivative : CIFEnableType = field(default_factory=lambda: CIFEnableType('second_derivative', template_type=True, packed_tag=PackedTag(22,1,0,0)))
    third_derivative : CIFEnableType = field(default_factory=lambda: CIFEnableType('third_derivative', template_type=True, packed_tag=PackedTag(21,1,0,0)))
    probability : CIFEnableType = field(default_factory=lambda: CIFEnableType('probability', type_=Probability(reserved_bits=16), packed_tag=PackedTag(20,1,0,0)))
    belief : CIFEnableType = field(default_factory=lambda: CIFEnableType('belief', type_=Belief(reserved_bits=24), packed_tag=PackedTag(19,1,0,0)))
    packed_0 : PackedType = PackedType('packed_0', bits=32, packed_tag=PackedTag(0,32,0))

    def __post_init__(self):
        super().__post_init__()

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            mode = parse_enable(val)
            if (mode == Mode.OPTIONAL):
                raise TypeError('invalid mode "optional" for {}'.format(key))
            self.__dict__[key].enabled = True
            self.__dict__[key].required = True

@dataclass
class CIF7(CIF):
    """
    CIF7 Enables
    """
    name : str = 'cif_7'
    attributes : CIF7Attributes = field(default_factory=lambda: CIF7Attributes('attributes', packed_tag=PackedTag(31,32,0,0)))

    def __post_init__(self):
        super().__post_init__()

    @property
    def has_enabled_fields(self):
        for field in self.attributes.fields:
            if isinstance(field, PackedType):
                continue
            if field.enabled:
                return True
        return False

    @property
    def all_optional_fields(self):
        return False

    @property
    def fields(self):
        return [self.attributes]

    def _validate(self, mapping):
        for field in mapping:
            if not field in [f.name for f in self.attributes.fields]:
                raise ValueError('invalid field provided: ' + field)

    @property
    def current_value_enabled(self):
        return self.attributes.current_value.enabled

    def _parse_mapping(self, mapping):
        for key,val in mapping.items():
            if key == 'current_value':
                print("Current Value is not supported by vrtgen")
            mode = parse_enable(val)
            if mode != Mode.REQUIRED:
                print('{} is a required field, ignoring: {}'.format(key, val))
            
            self.attributes.__dict__[key].enabled = True
            self.attributes.__dict__[key].required = True

