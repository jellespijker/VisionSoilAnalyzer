#!/bin/bash
# Convert the code files to one big pdf

cd ..
cd src

folders="VSAGUI SoilMath SoilHardware SoilVision Soil Tests"

for project in $folders
do
    cd "$project"
    find . -type f \( -iname \*.h -o -iname \*.cpp \) | xargs enscript --color=1 -C -Ecpp -fCourier8 -o - | ps2pdf - "$project".pdf
    mv -f "$project".pdf ../../Documentation/codepdf/
    cd ..
done

