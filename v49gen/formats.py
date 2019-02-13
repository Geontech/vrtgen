from enum import IntEnum

# TSI Code
# 00 - no integer timecode
# 01 - UTC
# 10 - GPS
# 11 - Other
class TSI(IntEnum):
    NONE  = 0
    UTC   = 1
    GPS   = 2
    OTHER = 3

# TSF Code
# 00 - no fractional timcode
# 01 - sample-count
# 10 - real-time (picoseconds)
# 11 - free-running count
class TSF(IntEnum):
    NONE         = 0
    SAMPLE_COUNT = 1
    REAL_TIME    = 2
    FREE_RUNNING = 3

class IntFormat:
    def __init__(self, bits=32):
        self.bits = bits

class FixedFormat(IntFormat):
    def __init__(self, bits, radix):
        super().__init__(bits)
        self.radix = radix

BIT = IntFormat(1)
INT32 = IntFormat(32)
INT64 = IntFormat(64)
