#!/bin/bash
find . -type f \( -iname \*.h -o -iname \*.cpp \) | xargs enscript --color=1 -C -Ecpp -fCourier8 -o - | ps2pdf - code.pdf
