#pragma once

#include <string>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "../Vision/Vision.h"

namespace SoilAnalyzer {
    class SoilSettings
    {
    public:
        SoilSettings();

        void SaveSettings(std::string filename);
        void LoadSettings(std::string filename);

        bool useAdaptiveContrast = true;
        uint32_t adaptContrastKernelSize = 9;
        float adaptContrastKernelFactor = 1.;

        bool useBlur = true;
        uint32_t blurKernelSize = 5;

        Vision::Segment::TypeOfObjects typeOfObjectsSegmented = Vision::Segment::Dark;
        bool ignorePartialBorderParticles = true;
        bool fillHoles = true;
        float sigmaFactor = 2;
        int thresholdOffsetValue = 0;

        Vision::MorphologicalFilter::FilterType morphFilterType = Vision::MorphologicalFilter::OPEN;
        uint32_t filterMaskSize = 5;

    private:
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(useAdaptiveContrast);
            ar & BOOST_SERIALIZATION_NVP(adaptContrastKernelFactor);
            ar & BOOST_SERIALIZATION_NVP(adaptContrastKernelSize);
            ar & BOOST_SERIALIZATION_NVP(useBlur);
            ar & BOOST_SERIALIZATION_NVP(blurKernelSize);
            ar & BOOST_SERIALIZATION_NVP(typeOfObjectsSegmented);
            ar & BOOST_SERIALIZATION_NVP(ignorePartialBorderParticles);
            ar & BOOST_SERIALIZATION_NVP(fillHoles);
            ar & BOOST_SERIALIZATION_NVP(sigmaFactor);
            ar & BOOST_SERIALIZATION_NVP(morphFilterType);
            ar & BOOST_SERIALIZATION_NVP(filterMaskSize);
            ar & BOOST_SERIALIZATION_NVP(thresholdOffsetValue);
        }
    };
}
