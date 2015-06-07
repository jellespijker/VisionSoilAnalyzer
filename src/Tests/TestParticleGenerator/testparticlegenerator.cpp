/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */


#include "testparticlegenerator.h"


TestParticleGenerator::TestParticleGenerator()
{
}

cv::Mat TestParticleGenerator::GenParticle(uint32_t size, float ellipseratio, cv::Vec3b meanColor, cv::Vec3f stdColor, float roughness, float angle)
{
    uint32_t height, width;
    height =
    cv::Mat img(x ? x : y, );
    cv::ellipse(img, cv::Point(img.cols/2, img.rows/2), cv::Size(size, size * ellipseratio), angle, 0 360, cv::Scalar(1,1,1), -1);

}
