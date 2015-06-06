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
