set -e
#clear test.exe
export CXX=clang++
cd src/co_/
pwd
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/MyCoroutineProject