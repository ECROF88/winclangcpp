set -e
#clear test.exe
export CXX=clang++
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/test



