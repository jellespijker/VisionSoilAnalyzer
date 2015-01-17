Soil
====

Project for my Bachelor Minor -> Embedded Vision Design @ University of Applied Sciences HAN Arnhem en Nijmegen. 
The Doxygen documentation can be found here: http://peer23peer.github.io/VisionSoilAnalyzer

The Soil Analyzer is a portable lab that can analyse soil samples and tells the user about it's optical properties. It looks at the texture and describe it as a Particle Size Distribution. This information is presented as a Cumulative Distribution Function (CDF). It also describes the structure of the soil sample and gives information on the sphericity and roundness / angularity. It does so by analysing the contour of particles, describing them as Fourier Descriptors and using a Neural Network to describe it's properties. Finally it gives the color properties in a histogram in the CIE La*b* colorspace, which show correlations with the fertility of the soil. and a histogram of the Redness Index, which gives has a correlation on the iron content.

The analyse is performed on a Beaglebone Black rev C. with the following image Linux arm 3.8.13-bone68 #1 SMP Sat Nov 22 06:51:58 UTC 2014 armv7l armv7l armv7l GNU/Linux

The soil is analyzed using it optical properties. This is done with a 5MP USB webcam with a maximum magnification of 300x. It has a custom cape with which the user can control the device.

Although it uses openCV most Vision algorithms are custom written (part of my Minor demand). openCV is used for camera aquisition and picture storage mostly. This project make use of custom vision, object detection and classification algorithmes.

Alot of research for this project was performed, mostly on the optical properties of Soil. This paper is written in Dutch an and will be translated in English for the wiki on a later time. The following sources were used in this project:

- Alvarez Grima, M. (2014, July 16). 
  Orientatie interview MTI.
- Ambrogi, F., Lama, N., Boracchi, P., & Biganzoli, E. (2007).
  Selection of artificial neural network models for survival analysis with Genetic Algorithms.
  Computational Statistics & Data Analysis, 52(1), 30–42. doi:10.1016/j.csda.2007.05.001 
- Andy Ward, Ph.D. (2012, October 25). 
  The Importance of Particle Size Distributions to The Characterization of Soils. 
- Blott, S. J., & Pye, K. (2001). 
  GRADISTAT: a grain size distribution and statistics package for the analysis of unconsolidated sediments.
  Earth Surface Processes and Landforms, 26(11), 1237–1248.
- Chesworth, W. (Ed.). (2008). 
  Encyclopedia of Soil Science. Dordrecht: Springer Netherlands.
- Cox, M. R., & Budhu, M. (2008).
  A practical approach to grain shape quantification.
  Engineering Geology, 96(1–2), 1–16. doi:10.1016/j.enggeo.2007.05.005 
- Gómez-Robledo, L., López-Ruiz, N., Melgosa, M., Palma, A. J., Capitán-Vallvey, L. F., & Sánchez-Marañón, M. (2013).
  Using the mobile phone as Munsell soil-colour sensor: An experiment under controlled illumination conditions.
  Computers and Electronics in Agriculture, 99, 200–208. doi:10.1016/j.compag.2013.10.002
- Hartemink, A. E., & Minasny, B. (2014). 
  Towards digital soil morphometrics. Geoderma, 230–231, 305–317. doi:10.1016/j.geoderma.2014.03.008 
- Igathinathane, C., Pordesimo, L. O., Columbus, E. P., Batchelor, W. D., & Sokhansanj, S. (2009). 
  Sieveless particle size distribution analysis of particulate materials through computer vision. 
  Computers and Electronics in Agriculture, 66(2), 147–158. doi:10.1016/j.compag.2009.01.005 
- Igathinathane, C., Ulusoy, U., & Pordesimo, L. O. (2012). 
  Comparison of particle size distribution of celestite mineral by machine vision ΣVolume approach and mechanical sieving. 
  Powder Technology, 215–216, 137–146. doi:10.1016/j.powtec.2011.09.037 
- Kröner, S., & Doménech Carbó, M. T. (2013).
  Determination of minimum pixel resolution for shape analysis: Proposal of a new data validation method for computerized images.
  Powder Technology, 245, 297–313. doi:10.1016/j.powtec.2013.04.048 
- Melville, M. D., & Atkinson, G. (1985). 
  Soil colour: its measurement and its designation in models of uniform kleurmodel. 
  Journal of Soil Science, 36(4), 495–512. doi:10.1111/j.1365-2389.1985.tb00353.x 
- Miller, N. A., & Henderson, J. J. (2010). 
  Quantifying Sand Particle Shape Complexity using a Dynamic, Digital Imaging Technique. 
  Agronomy Journal, 102(5), 1407. doi:10.2134/agronj2010.0097 
- Phokharatkul, P., & Kimpan, C. (2002). 
  Handwritten Thai Character Recognition Using Fourier Descriptors and Genetic Neural Networks. 
  Computational Intelligence, 18(3), 270–293. 
- Raj, P. M., & Cannon, W. R. (1999). 
  2-D particle shape averaging and comparison using Fourier descriptors. 
  Powder Technology, 104(2), 180–189. doi:10.1016/S0032-5910(99)00046-7 
- Soil. (2014, September 3). 
  In Wikipedia, the free encyclopedia. 
  Retrieved from https://en.wikipedia.org/w/index.php?title=Soil&oldid=624015984 
- Training Institute for dredging. (2008). 
  Ingewijde Training. Kinderdijk: MTI. 
- Van Elburg, J. (2014, September 23). 
  Hoe omgaan met PSD en het bepalen van een minimale sample groote. 
- Viscarra Rossel, R. A., Fouad, Y., & Walter, C. (2008). 
  Using a digital camera to measure soil organic carbon and iron contents. 
  Biosystems Engineering, 100(2), 149–159. doi:10.1016/j.biosystemseng.2008.02.007 
- Viscarra Rossel, R. A., Minasny, B., Roudier, P., & McBratney, A. B. (2006). 
  Kleurmodel models for soil science. Geoderma, 133(3–4), 320–337. doi:10.1016/j.geoderma.2005.07.017 
- Xu, K., Luxmoore, A. R., Jones, L. M., & Deravi, F. (1998). 
  Integration of neural networks and expert systems for microscopic wear particle analysis. 
  Knowledge-Based Systems, 11(3–4), 213–227. doi:10.1016/S0950-7051(98)00052-5
