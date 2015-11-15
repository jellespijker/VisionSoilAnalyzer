#!bin/bash
#script to build Vision Soil Analyzer

START=$(date +%s)

config = $1

echo "Building Vision Soil Analyzer"
./setupbuilddir.sh

cd ../build/$config/SoilMath/
rm *
qmake ../../../src/SoilMath/SoilMath.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../SoilHardware/
rm *
qmake ../../../src/SoilHardware/SoilHardw.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../SoilVision/
rm *
qmake ../../../src/SoilVision/SoilVision.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../SoilAnalyzer/
rm *
qmake ../../../src/SoilAnalyzer/SoilAnalyzer.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../../release/pictureflow-qt/
rm *
qmake ../../../src/pictureflow/lib.pro -r -spec linux-g++
make
make install

cd ../qcustomplot/
rm *
qmake ../../../src/qcustomplot/qcustomplot.pro -r -spec linux-g++
make
make install

cd ../../$config/QOpenCVQT/
rm *
qmake ../../../src/QOpenCVQT/QOPEnCVQT.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../QParticleDisplay/
rm *
qmake ../../../src/QParticleDisplay/QParticleDisplay.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../QParticleSelector/
rm *
qmake ../../../src/QParticleSelector/QParticleSelector.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../QReportGenerator
rm *
qmake ../../../src/QReportGenerator/QReportGenerator.pro -r -spec linux-g++ CONFIG+=$config
make
make install

cd ../VSA
rm *
qmake ../../../src/VSA/VSA.pro -r -spec linux-g++ CONFIG+=$config
make
make install

END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Building took $DIFF seconds"