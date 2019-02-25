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

class SSI(IntEnum):
    """
    Start/Stop of Sample Frame Indication (SSI) Bits:
      SINGLE (00) - Sample Frames are not applicable to data packets, or the
                    entire Sample Frame is contained in a single data packet
      FIRST (01)  - First data packet of current Sample Frame
      MIDDLE (10) - Middle packet or packets of Sample Frame: "continuation"
                    indicator
      FINAL (11)  - Final data packet of current Sample Frame
    """
    SINGLE = 0
    FIRST = 1
    MIDDLE = 2
    FINAL = 3
