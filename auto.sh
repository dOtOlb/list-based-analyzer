#!/bin/sh

cd /projects/list_based_analyzer/src
g++ -std=c++11 -c ChainAnalyzer.cpp
g++ -std=c++11 -c GeoPerfAnalyzer.cpp
g++ -std=c++11 -c TxSetCalculator.cpp
g++ -std=c++11 -c TimeCurve.cpp
g++ -std=c++11 -c analyzermain.cpp
g++ -std=c++11 -o analyzermain.exe *.o
gzip -cd ../test/samplelog2.gz | /Users/ccheng/Downloads/Akamai/work/cc_log_convert | ./analyzermain.exe -i ../test/test2.csv -g ../test/test.csv
