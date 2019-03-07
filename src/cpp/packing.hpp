#ifndef _VRTGEN_PACKING_HPP_
#define _VRTGEN_PACKING_HPP_

#include <inttypes.h>

#define SET_BIT(x,bit,enable)
#define GET_BIT(x,bit) false
namespace vrtgen {
    
    namespace PacketType {
        /**
         * Constants for the 4-bit Packet Type field in the VRT Packet Header.
         * Refer to VITA 49.2 Table 5.1.1-1.
         */
        enum Code {
            SIGNAL_DATA = 0b0000,
            SIGNAL_DATA_STREAM_ID = 0b0001,
            EXTENSION_DATA = 0b0010,
            EXTENSION_DATA_STREAM_ID = 0b0011,
            CONTEXT = 0b0100,
            EXTENSION_CONTEXT = 0b0101,
            COMMAND = 0b0110,
            EXTENSION_COMMAND = 0b0111,
        };
    }
    
    namespace TSI {
        /**
         * TimeStamp-Integer (TSI) codes:
         *     NONE  (00) - No Integer-seconds Timestamp field included
         *     UTC   (01) - Coordinated Universal Time
         *     GPS   (10) - GPS time
         *     OTHER (11) - Other, must be documented
         */
        enum Code {
            NONE = 0b00,
            UTC = 0b01,
            GPS = 0b10,
            OTHER = 0b11,
        };
    }
    
    namespace TSF {
        /**
         * TimeStamp-Fractional (TSF) codes:
         *     NONE         (00) - No Fractional-seconds Timestamp field included
         *     SAMPLE_COUNT (01) - Sample Count Timestamp
         *     REAL_TIME    (10) - Real-Time (Picoseconds) Timestamp
         *     FREE_RUNNING (11) - Free Running Count Timestamp
         */
        enum Code {
            NONE = 0b00,
            SAMPLE_COUNT = 0b01,
            REAL_TIME = 0b10,
            FREE_RUNNING = 0b11,
        };
    }
    
    namespace SSI {
        /**
         * Start/Stop of Sample Frame Indication (SSI) Bits:
         *     SINGLE (00) - Sample Frames are not applicable to data packets, or the
         *                   entire Sample Frame is contained in a single data packet
         *     FIRST  (01) - First data packet of current Sample Frame
         *     MIDDLE (10) - Middle packet or packets of Sample Frame: "continuation"
         *                   indicator
         *     FINAL  (11) - Final data packet of current Sample Frame
         */
        enum Code {
            SINGLE = 0b00,
            FIRST = 0b01,
            MIDDLE = 0b10,
            FINAL = 0b11,
        };
    }
    namespace packing {
        
        struct CIF0Prologue {
            uint32_t word;
            };
            
            bool getBandwidth() const
            {
                return GET_BIT(word, 29);
            }
            
            void setBandwidth(bool enable)
            {
                SET_BIT(word, 29, enable);
            }
            
            bool getContextAssociationLists() const
            {
                return GET_BIT(word, 8);
            }
            
            void setContextAssociationLists(bool enable)
            {
                SET_BIT(word, 8, enable);
            }
            
            bool getSignalDataPacketPayloadFormat() const
            {
                return GET_BIT(word, 15);
            }
            
            void setSignalDataPacketPayloadFormat(bool enable)
            {
                SET_BIT(word, 15, enable);
            }
            
            bool getDeviceIdentifier() const
            {
                return GET_BIT(word, 17);
            }
            
            void setDeviceIdentifier(bool enable)
            {
                SET_BIT(word, 17, enable);
            }
            
            bool getECEFEphemeris() const
            {
                return GET_BIT(word, 12);
            }
            
            void setECEFEphemeris(bool enable)
            {
                SET_BIT(word, 12, enable);
            }
            
            bool getEphemerisReferenceID() const
            {
                return GET_BIT(word, 10);
            }
            
            void setEphemerisReferenceID(bool enable)
            {
                SET_BIT(word, 10, enable);
            }
            
            bool getFormattedGPS() const
            {
                return GET_BIT(word, 14);
            }
            
            void setFormattedGPS(bool enable)
            {
                SET_BIT(word, 14, enable);
            }
            
            bool getFormattedINS() const
            {
                return GET_BIT(word, 13);
            }
            
            void setFormattedINS(bool enable)
            {
                SET_BIT(word, 13, enable);
            }
            
            bool getGain() const
            {
                return GET_BIT(word, 23);
            }
            
            void setGain(bool enable)
            {
                SET_BIT(word, 23, enable);
            }
            
            bool getGPSASCII() const
            {
                return GET_BIT(word, 9);
            }
            
            void setGPSASCII(bool enable)
            {
                SET_BIT(word, 9, enable);
            }
            
            bool getIFBandOffset() const
            {
                return GET_BIT(word, 25);
            }
            
            void setIFBandOffset(bool enable)
            {
                SET_BIT(word, 25, enable);
            }
            
            bool getIFReferenceFrequency() const
            {
                return GET_BIT(word, 28);
            }
            
            void setIFReferenceFrequency(bool enable)
            {
                SET_BIT(word, 28, enable);
            }
            
            bool getOverRangeCount() const
            {
                return GET_BIT(word, 22);
            }
            
            void setOverRangeCount(bool enable)
            {
                SET_BIT(word, 22, enable);
            }
            
            bool getReferenceLevel() const
            {
                return GET_BIT(word, 24);
            }
            
            void setReferenceLevel(bool enable)
            {
                SET_BIT(word, 24, enable);
            }
            
            bool getReferencePointIdentifier() const
            {
                return GET_BIT(word, 30);
            }
            
            void setReferencePointIdentifier(bool enable)
            {
                SET_BIT(word, 30, enable);
            }
            
            bool getRelativeEphemeris() const
            {
                return GET_BIT(word, 11);
            }
            
            void setRelativeEphemeris(bool enable)
            {
                SET_BIT(word, 11, enable);
            }
            
            bool getRFReferenceFrequency() const
            {
                return GET_BIT(word, 27);
            }
            
            void setRFReferenceFrequency(bool enable)
            {
                SET_BIT(word, 27, enable);
            }
            
            bool getRFReferenceFrequencyOffset() const
            {
                return GET_BIT(word, 26);
            }
            
            void setRFReferenceFrequencyOffset(bool enable)
            {
                SET_BIT(word, 26, enable);
            }
            
            bool getSampleRate() const
            {
                return GET_BIT(word, 21);
            }
            
            void setSampleRate(bool enable)
            {
                SET_BIT(word, 21, enable);
            }
            
            bool getStateEventIndicators() const
            {
                return GET_BIT(word, 16);
            }
            
            void setStateEventIndicators(bool enable)
            {
                SET_BIT(word, 16, enable);
            }
            
            bool getTemperature() const
            {
                return GET_BIT(word, 18);
            }
            
            void setTemperature(bool enable)
            {
                SET_BIT(word, 18, enable);
            }
            
            bool getTimestampAdjustment() const
            {
                return GET_BIT(word, 20);
            }
            
            void setTimestampAdjustment(bool enable)
            {
                SET_BIT(word, 20, enable);
            }
            
            bool getTimestampCalibrationTime() const
            {
                return GET_BIT(word, 19);
            }
            
            void setTimestampCalibrationTime(bool enable)
            {
                SET_BIT(word, 19, enable);
            }
        };
        
        struct CIF1Prologue {
            uint32_t word;
            };
            
            bool getArrayofCIFS() const
            {
                return GET_BIT(word, 11);
            }
            
            void setArrayofCIFS(bool enable)
            {
                SET_BIT(word, 11, enable);
            }
            
            bool getAuxBandwidth() const
            {
                return GET_BIT(word, 13);
            }
            
            void setAuxBandwidth(bool enable)
            {
                SET_BIT(word, 13, enable);
            }
            
            bool getAuxFrequency() const
            {
                return GET_BIT(word, 15);
            }
            
            void setAuxFrequency(bool enable)
            {
                SET_BIT(word, 15, enable);
            }
            
            bool getAuxGain() const
            {
                return GET_BIT(word, 14);
            }
            
            void setAuxGain(bool enable)
            {
                SET_BIT(word, 14, enable);
            }
            
            bool getBeamWidths() const
            {
                return GET_BIT(word, 25);
            }
            
            void setBeamWidths(bool enable)
            {
                SET_BIT(word, 25, enable);
            }
            
            bool getBufferSize() const
            {
                return GET_BIT(word, 1);
            }
            
            void setBufferSize(bool enable)
            {
                SET_BIT(word, 1, enable);
            }
            
            bool getCompressionPoint() const
            {
                return GET_BIT(word, 18);
            }
            
            void setCompressionPoint(bool enable)
            {
                SET_BIT(word, 18, enable);
            }
            
            bool getDiscreteIO32() const
            {
                return GET_BIT(word, 6);
            }
            
            void setDiscreteIO32(bool enable)
            {
                SET_BIT(word, 6, enable);
            }
            
            bool getDiscreteIO64() const
            {
                return GET_BIT(word, 5);
            }
            
            void setDiscreteIO64(bool enable)
            {
                SET_BIT(word, 5, enable);
            }
            
            bool getEbNoBER() const
            {
                return GET_BIT(word, 20);
            }
            
            void setEbNoBER(bool enable)
            {
                SET_BIT(word, 20, enable);
            }
            
            bool getHealthStatus() const
            {
                return GET_BIT(word, 4);
            }
            
            void setHealthStatus(bool enable)
            {
                SET_BIT(word, 4, enable);
            }
            
            bool getIndexList() const
            {
                return GET_BIT(word, 7);
            }
            
            void setIndexList(bool enable)
            {
                SET_BIT(word, 7, enable);
            }
            
            bool getInterceptPoints() const
            {
                return GET_BIT(word, 17);
            }
            
            void setInterceptPoints(bool enable)
            {
                SET_BIT(word, 17, enable);
            }
            
            bool getPhaseOffset() const
            {
                return GET_BIT(word, 31);
            }
            
            void setPhaseOffset(bool enable)
            {
                SET_BIT(word, 31, enable);
            }
            
            bool get3DPointingVector() const
            {
                return GET_BIT(word, 29);
            }
            
            void set3DPointingVector(bool enable)
            {
                SET_BIT(word, 29, enable);
            }
            
            bool get3DPointingVectorStructure() const
            {
                return GET_BIT(word, 28);
            }
            
            void set3DPointingVectorStructure(bool enable)
            {
                SET_BIT(word, 28, enable);
            }
            
            bool getPolarization() const
            {
                return GET_BIT(word, 30);
            }
            
            void setPolarization(bool enable)
            {
                SET_BIT(word, 30, enable);
            }
            
            bool getRange() const
            {
                return GET_BIT(word, 24);
            }
            
            void setRange(bool enable)
            {
                SET_BIT(word, 24, enable);
            }
            
            bool getSNRNoiseFigure() const
            {
                return GET_BIT(word, 16);
            }
            
            void setSNRNoiseFigure(bool enable)
            {
                SET_BIT(word, 16, enable);
            }
            
            bool getSpatialReferenceType() const
            {
                return GET_BIT(word, 26);
            }
            
            void setSpatialReferenceType(bool enable)
            {
                SET_BIT(word, 26, enable);
            }
            
            bool getSpatialScanType() const
            {
                return GET_BIT(word, 27);
            }
            
            void setSpatialScanType(bool enable)
            {
                SET_BIT(word, 27, enable);
            }
            
            bool getSpectrum() const
            {
                return GET_BIT(word, 10);
            }
            
            void setSpectrum(bool enable)
            {
                SET_BIT(word, 10, enable);
            }
            
            bool getThreshold() const
            {
                return GET_BIT(word, 19);
            }
            
            void setThreshold(bool enable)
            {
                SET_BIT(word, 19, enable);
            }
            
            bool getVersionandBuildCode() const
            {
                return GET_BIT(word, 2);
            }
            
            void setVersionandBuildCode(bool enable)
            {
                SET_BIT(word, 2, enable);
            }
        };
    }
}
#endif // _VRTGEN_PACKING_HPP_
