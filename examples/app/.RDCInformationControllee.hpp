#ifndef RDCINFORMATIONCONTROLLEE_HPP
#define RDCINFORMATIONCONTROLLEE_HPP

#include <stdexcept>
#include "Streamer.hpp"
#include "RDCInformationControllee_base.hpp"

class RDCInformationControllee : public RDCInformationControllee_base
{

public:
    RDCInformationControllee() = default;
    virtual ~RDCInformationControllee() = default;

    virtual double getBandwidth()
    {
        // AUTO-GENERATED FUNCTION STUB
        // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
        throw std::runtime_error("getBandwidth not implemented");
    }

    virtual double getRFReferenceFrequency()
    {
        return streamer.getFrequency();
    }

    virtual vrtgen::packing::Gain getGain()
    {
        // AUTO-GENERATED FUNCTION STUB
        // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
        throw std::runtime_error("getGain not implemented");
    }

    virtual double getSampleRate()
    {
        // AUTO-GENERATED FUNCTION STUB
        // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
        throw std::runtime_error("getSampleRate not implemented");
    }

    virtual void setBandwidth(const double& value)
    {
        // AUTO-GENERATED FUNCTION STUB
        // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
        throw std::runtime_error("setBandwidth not implemented");
    }

    virtual void setRFReferenceFrequency(const double& value)
    {
        streamer.setFrequency(value);
    }

    virtual void setGain(const vrtgen::packing::Gain& value)
    {
        streamer.setGain(value.getStage1());
    }

    virtual void setSampleRate(const double& value)
    {
        streamer.setSampleRate(value);
    }

    virtual void setDiscreteIO32(const ::structs::configuretuner::DiscreteIO32& value,
                                 const endpoint_type& controllerDataEndpoint)
    {
        if (value.isStreamEnableEnabled()) {
            streamer.setControllerDataEndpoint(controllerDataEndpoint);
            if (value.getStreamEnable()) {
                streamer.startStream();
            } else {
                streamer.stopStream();
            }
        }
    }

private:
    Streamer streamer;

}; // end class RDCInformationControllee

#endif // RDCINFORMATIONCONTROLLEE_HPP
