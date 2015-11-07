 
#!bin/bash
#script to build Vision Soil Analyzer


START=$(date +%s)

echo "Building Vision Soil Analyzer"
setupbuilddir.sh

cd ../build/debug/SoilMath/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/SoilMath/SoilMath.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../SoilHardware/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/SoilHardware/SoilHardw.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../SoilVision/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/SoilVision/SoilVision.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../SoilAnalyzer/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/SoilAnalyzer/SoilAnalyzer.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../../release/pictureflow-qt/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/pictureflow/lib.pro -r -spec linux-g++
make
make install

cd ../qcustomplot/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/qcustomplot/qcustomplot.pro -r -spec linux-g++
make
make install

cd ../../debug/QOpenCVQT/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/QOpenCVQT/QOPEnCVQT.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../QParticleDisplay/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/QParticleDisplay/QParticleDisplay.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../QParticleSelector/
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/QParticleSelector/QParticleSelector.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../QReportGenerator
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/QReportGenerator/QReportGenerator.pro -r -spec linux-g++ CONFIG+=debug
make
make install

cd ../VSA
rm *
qmake /home/peer23peer/ihc/VisionSoilAnalyzer/src/VSA/VSA.pro -r -spec linux-g++ CONFIG+=debug
make
make install

END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Building took $DIFF seconds"