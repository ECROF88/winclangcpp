set -e
#clear test.exe
#进入根目录
export CXX=clang++
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/test



