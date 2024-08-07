set -e

export CXX=clang++
cd src/LC/
rm  300
clang++ -std=c++17 300.cpp -o 300
pwd
./300.exe