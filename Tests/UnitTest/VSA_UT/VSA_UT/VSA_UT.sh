#!/bin/sh
#Unit Testing with common options argument is the unit test filter 

./Debug/VSA_UT --log_level=all --report_level=detailed --build_info=yes --show_progress=yes --run_test=$1
