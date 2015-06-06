#ifndef TESTPARTICLEGENERATOR_H
#define TESTPARTICLEGENERATOR_H

#include "testparticlegenerator_global.h"
#include <opencv2/core.hpp>
#include <vector>
#include <stdint.h>

class TESTPARTICLEGENERATORSHARED_EXPORT TestParticleGenerator
{


public:
    std::vector<cv::Mat> particles;
    TestParticleGenerator();
    cv::Mat GenParticle(uint32_t size, float ellipseratio, cv::Vec3b meanColor, cv::Vec3f stdColor, float roughness, float angle);
};

#endif // TESTPARTICLEGENERATOR_H
