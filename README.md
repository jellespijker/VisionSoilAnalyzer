Soil
====

Project for my Bachelor Minor -> Embedded Vision Design and my graduation project -> Mechanical ENgineering @ University of Applied Sciences HAN Arnhem en Nijmegen.

The Doxygen documentation can be found here: http://peer23peer.github.io/VisionSoilAnalyzer/Webpage/index.html
Further documentation indepth background information can be found at the wikie
Youtube movie of the first testrun of the SoilCape can be found here: https://www.youtube.com/watch?v=Ew5GgzQKrGk

The Soil Analyzer is a portable lab that can analyse soil samples and tells the user about it's optical properties. It looks at the texture and describe it as a Particle Size Distribution. This information is presented as a Cumulative Distribution Function (CDF). It also describes the structure of the soil sample and gives information on the sphericity and roundness / angularity. It does so by analysing the contour of particles, describing them as Fourier Descriptors and using a Neural Network to describe it's properties. Finally it gives the color properties in a histogram in the CIE La*b* colorspace, which show correlations with the fertility of the soil. and a histogram of the Redness Index, which gives has a correlation on the iron content.

The analyse is performed on a Beaglebone Black rev C. with the following image Linux arm 3.8.13-bone68 #1 SMP Sat Nov 22 06:51:58 UTC 2014 armv7l armv7l armv7l GNU/Linux

The soil is analyzed using it optical properties. This is done with a 5MP USB webcam with a maximum magnification of 300x. It has a custom cape with which the user can control the device.

Although it uses openCV most Vision algorithms are custom written (part of my Minor demand). openCV is used for camera aquisition and picture storage mostly. This project make use of custom vision, object detection and classification algorithmes.

test