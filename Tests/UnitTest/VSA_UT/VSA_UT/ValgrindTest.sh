#!/bin/sh
#Valgrind Testing script output xml file

valgrind --tool=memcheck --leak-check=full --xml=yes --xml-file=valgrind.log ./Debug/VSA_UT --run_test=$1
