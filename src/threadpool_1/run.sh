set -e
#clear test.exe
export CXX=clang++
pwd
cd src/threadpool_1/
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/threadpool




